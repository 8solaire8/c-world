#include <stdio.h>

#include "types.h"
#include "platform.h"
#include "lane.h"

static thread_local LaneContext g_lane_context;

u64
lane_idx()
{
    return g_lane_context.id;
}

u64
lane_count()
{
    return g_lane_context.count;
}

void 
lane_sync()
{
    barrier_wait(g_lane_context.barrier);
}

LaneRange 
lane_range(u64 values_count)
{
    u64 thread_idx = lane_idx();
    u64 thread_count = lane_count();
    u64 values_per_thread = values_count / thread_count;
    u64 leftover_values_count = values_count % thread_count;
    u64 thread_has_leftover = (thread_idx < leftover_values_count);
    u64 leftovers_before_this_thread_idx = (thread_has_leftover
                                            ? thread_idx
                                            : leftover_values_count);
    u64 thread_first_value_idx = (values_per_thread * thread_idx +
                                  leftovers_before_this_thread_idx);
    u64 thread_opl_value_idx = (thread_first_value_idx + values_per_thread +
                                !!thread_has_leftover);
    
    return (LaneRange) {
        .min = thread_first_value_idx, .max = thread_opl_value_idx
    };
}

void
lane_sync_u64(u64* lane_local_storage, u32 broadcast_src_lane_idx)
{
    u64 broadcast_size = 8;
    
    if (lane_idx() == broadcast_src_lane_idx)
    {
        CopyMemory(g_lane_context.shared_buffer, lane_local_storage, broadcast_size);
    }
    lane_sync();
    
    if (lane_idx() != broadcast_src_lane_idx)
    {
        CopyMemory(lane_local_storage, g_lane_context.shared_buffer, broadcast_size);
    }
    lane_sync();
}

DWORD WINAPI 
EntryPoint(LPVOID parameter)
{
    LaneContext* args = (LaneContext*)parameter;
    
    g_lane_context.id = args->id;
    g_lane_context.count = args->count;
    g_lane_context.barrier = args->barrier;
    g_lane_context.shared_buffer = args->shared_buffer;
    
    if (lane_idx() == 0) 
    {
        printf("hello world\n");
    }
    
    LaneRange lr = lane_range(65);
    
    for (int i = lr.min; i < lr.max; ++i)
    {
        
    }
    
    u64 value = 30;
    
    if (lane_idx() == 0) 
    {
        value = 10;
    }
    
    lane_sync_u64(&value, 0);
    
    if (lane_idx() == 10) 
    {
        printf("value = %ld\n", value);
    }
    
    if (lane_idx() == 0) 
    {
        printf("LaneRanges: min = %d, max = %d\n", lr.min, lr.max);
    }
    lane_sync();
    
}

static u64 shared_buffer;
static Barrier barrier;

int 
run_test()
{
    
    DWORD count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    
    Barrier b = {0};
    
    if (barrier_init(&barrier, CPU_NUMBER))
    {
        return 1;
    }
    
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
    
	return 0;
}