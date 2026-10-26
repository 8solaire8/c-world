#include <Windows.h>
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include "defs.h"
#include "types.h"
#include "wave.h"
#include "lane.h"
#include "arena.h"

#define CPU_NUMBER 32
#define FILTER_TAP_NUM 21


static F32 filter_taps[FILTER_TAP_NUM] = 
{
  -0.02010411882885732,
  -0.05842798004352509,
  -0.061178403647821976,
  -0.010939393385338943,
  0.05125096443534972,
  0.033220867678947885,
  -0.05655276971833928,
  -0.08565500737264514,
  0.0633795996605449,
  0.31085440365663597,
  0.4344309124179415,
  0.31085440365663597,
  0.0633795996605449,
  -0.08565500737264514,
  -0.05655276971833928,
  0.033220867678947885,
  0.05125096443534972,
  -0.010939393385338943,
  -0.061178403647821976,
  -0.05842798004352509,
  -0.02010411882885732
};


#define FILTER_TAP_NUM 21

typedef struct {
    F32* taps;
    F32 history[FILTER_TAP_NUM - 1];
    U32 tap_count;
} FIR_State;


void
FIR_filter(FIR_State* s, F32* output, F32* input, U64 buffer_size)
{
    s->tap_count = FILTER_TAP_NUM;

    assert(buffer_size >= s->tap_count);
    assert(s->tap_count > 0);

    U64 pairs = s->tap_count / 2;
    U64 tap_num = s->tap_count;
    U64 history_num = s->tap_count - 1;

    for (U64 n = 0; n < history_num; ++n)
    {
        F32 temp_sum = 0;
        for (U64 k = 0; k < pairs; ++k)
        {
            F32 a = (k > n)
                ? s->history[history_num + n - k]
                : input[n - k];

            F32 b = (history_num - k > n)
                ? s->history[n + k]
                : input[n - (history_num - k)];

            temp_sum += (a + b) * filter_taps[k];
        }

        if (tap_num & 1)
        {
            F32 mid = (pairs > n)
                ? s->history[history_num + n - pairs]
                : input[n - pairs];

            temp_sum += mid * filter_taps[pairs];
        }

        output[n] = temp_sum;
    }

    for (U64 n = history_num; n < buffer_size; ++n)
    {
        F32 temp_sum = 0;

        for (U64 k = 0; k < pairs; ++k)
        {
            temp_sum += (input[n - k] + input[n - (history_num - k)]) * filter_taps[k];
        }
        if (tap_num & 1)
        {
            temp_sum += input[n - pairs] * filter_taps[pairs];
        }

        output[n] = temp_sum;
    }
    memcpy(s->history, input + buffer_size - history_num, history_num * sizeof(F32));
}

void 
stereo_to_mono(F32* output, const F32* input, U64 frame_count)
{
    for (U64 i = 0; i < frame_count; ++i)
    {
        F32 left = input[i * 2];
        F32 right = input[i * 2 + 1];

        output[i] = (left + right) * 0.5f;
    }
}


static U64 shared_buffer;
static Barrier barrier;

DWORD WINAPI 
EntryPoint(LPVOID parameter)
{
    LaneContext* args = (LaneContext*)parameter;

    g_lane_context.id            = args->id;
    g_lane_context.count         = args->count;
    g_lane_context.barrier       = args->barrier;
    g_lane_context.shared_buffer = args->shared_buffer;

    arena_create(&g_lane_context.arena, PAGES(5000));

    U32 channels;
    U32 sample_rate;
    U64 frame_number;
    drwav wav;

    if (!drwav_init_file(&wav, "input.wav", NULL))
        return 1;

    drwav_uninit(&wav);

    frame_number = wav.totalPCMFrameCount;

    F32* samples = NULL;

    if (lane_idx() == 0)
    {    

        samples =
            drwav_open_file_and_read_pcm_frames_f32(
                "input.wav",
                &channels,
                &sample_rate,
                &frame_number,
                NULL
            );
        printf("channels: %lu, sample rate: %lu, frame number: %llu\n", channels, sample_rate, frame_number );
    }

    lane_sync_u64(&samples, 0);

    F32* mono_out = NULL;

    if (lane_idx() == 0)
    {
        mono_out = arena_alloc(&g_lane_context.arena, frame_number * sizeof(F32));
    }

    lane_sync_u64(&mono_out, 0);

    LaneRange lr = lane_range(frame_number);

    if (lane_idx() == 0) {
        printf("LaneRanges: min = %d, max = %d\n", lr.min, lr.max);
    }

    stereo_to_mono(mono_out + lr.min, samples + lr.min, lane_width(lr));

    lane_sync();


    FIR_State fir = { 0 };
    F32* output = NULL;
    
    if (lane_idx() == 0)
    {
        output = arena_alloc(&g_lane_context.arena, frame_number * sizeof(F32));
    }
    
    lane_sync_u64(&output, 0);

    //printf("Address: %p\n", (void*)output);

    FIR_filter(&fir, output + lr.min, mono_out + lr.min, lane_width(lr));

    lane_sync();
    arena_clear(&g_lane_context.arena);
}


static UINT64 shared_buffer;
static Barrier barrier;

int 
main(int argv, char* argc)
{  
    DWORD count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);


    barrier_init(&barrier, CPU_NUMBER);

    HANDLE hThread[CPU_NUMBER];
    LaneContext ctx[CPU_NUMBER];

    for (int i = 0; i < CPU_NUMBER; ++i)
    {
        ctx[i].id = i;
        ctx[i].count = CPU_NUMBER;
        ctx[i].barrier = &barrier;
        ctx[i].shared_buffer = &shared_buffer;
        hThread[i] = CreateThread(
            NULL,              // Default security
            0,                 // Default stack size
            EntryPoint,        // Thread function
            (LPVOID) & ctx[i], // Parameter
            0,                 // Run immediately
            NULL               // Thread ID
        );
    }

    DWORD waitResult = WaitForMultipleObjects(
        CPU_NUMBER,    // Count of handles
        hThread,         // Array of handles
        TRUE,           // TRUE = Wait for ALL objects
        INFINITE        // Wait indefinitely
    );
	return 0;
}