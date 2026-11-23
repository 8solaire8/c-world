#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "dxguid.lib")

const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E,0x3D,0xC4,0x57,0x92,0x91,0x69,0x2E}};
const IID   IID_IMMDeviceEnumerator  = {0xA95664D2, 0x9614, 0x4F35, {0xA7,0x46,0xDE,0x8D,0xB6,0x36,0x17,0xE6}};
const IID   IID_IAudioClient         = {0x1CB9AD4C, 0xDBFA, 0x4C32, {0xB1,0x78,0xC2,0xF5,0x68,0xA7,0x03,0xB2}};
const IID   IID_IAudioRenderClient   = {0xF294ACFC, 0x3146, 0x4483, {0xA7,0xBF,0xAD,0xDC,0xA7,0xC2,0x60,0xE2}};



static IMMDeviceEnumerator* enumerator = NULL;
static IMMDevice* renderer = NULL;
static IAudioClient* renderClient = NULL;
static IAudioRenderClient* renderService = NULL;
static HANDLE audio_event;
static WAVEFORMATEX* format = NULL;

static Sound* g_sounds;
static AudioCmd audio_cmd;
static PlayingSound* playing_sounds;



void
gen_sign_wave_buff(F32 frequency, 
                   F32* samples,
                   F32* phase,
                   U32 channels,
                   U64 nFrames, 
                   F32 samples_per_second)
{
    
    for (U64 i = 0; i < nFrames; ++i) 
    {
        F32 sample = sinf(*phase) * 0.1f;
        
        for (U64 c = 0; c < channels; ++c ) 
        {
            samples[i * channels + c] = sample;
        }
        
        *phase += ((F32)M_TAU * frequency) / samples_per_second;
        
        if (*phase >= M_TAU) 
        {
            *phase -= (F32)M_TAU;
        }
    }
}

int is_audio_file(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    return ext &&
    (_stricmp(ext, ".ogg") == 0 || _stricmp(ext, ".wav") == 0);
}

void
load_sounds(Arena* arena)
{
    ArenaTemp temp = scratch_begin();
    
    WIN32_FIND_DATAA entry;
    HANDLE find = FindFirstFileA("res\\sound\\*", &entry);
    
    assert(find != INVALID_HANDLE_VALUE);
    
    g_sounds = push_array_zero(arena, Sound, 256);
    
    U32 sound_idx = 0;
    do {
        if (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        }
        if (is_audio_file(entry.cFileName))
        {
            // open file and read it into sound
            Sound* s = &g_sounds[sound_idx++];
            s->id = sound_idx;
            String8 str = str8(entry.cFileName);
            s->size = wav_size(str);
            
            s->data = push_array(arena, F32, s->size);
            wav_load(s->data, str);
        }
        
    } while (FindNextFileA(find, &entry));
    FindClose(find);
    
    scratch_end(temp);
}

void 
play_sound(U32 id)
{
    
}


// TODO: have it work if sound source changes
// TODO: implementing of the pushing of audio
bool
audio_create() 
{
    audio_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(audio_event == NULL) return false;
    
    HRESULT hr;
    
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    assert(SUCCEEDED(hr));
    
    hr = CoCreateInstance(
                          &CLSID_MMDeviceEnumerator,
                          NULL,
                          CLSCTX_ALL,
                          &IID_IMMDeviceEnumerator,
                          (void**)&enumerator
                          );
    assert(SUCCEEDED(hr));
    
    hr = IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator, eRender, eConsole, &renderer);
    if(!SUCCEEDED(hr)) return false;
    
    IMMDeviceEnumerator_Release(enumerator);
    
    if(!SUCCEEDED(hr)) return false;
    
    hr = IMMDevice_Activate(renderer, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&renderClient);
    
    if(!SUCCEEDED(hr)) return false;
    
    hr = IAudioClient_GetMixFormat(renderClient, &format);
    
    if(!SUCCEEDED(hr)) return false;
    
    hr = IAudioClient_Initialize(renderClient, 
                                 AUDCLNT_SHAREMODE_SHARED,
                                 AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
                                 0, 0,
                                 format,
                                 NULL);
    
    if(!SUCCEEDED(hr)) return false;
    
    hr = IAudioClient_SetEventHandle(renderClient, audio_event);
    
    if(!SUCCEEDED(hr)) return false;
    
    
    hr = IAudioClient_GetService(renderClient, &IID_IAudioRenderClient, (void**)&renderService);
    
    if(!SUCCEEDED(hr)) return false;
    return true;
}


DWORD WINAPI
audio_thread(LPVOID arg)
{
    HRESULT hr;
    hr = IAudioClient_Start(renderClient);
    
    if(!SUCCEEDED(hr))
    {
        return 1;
    }
    
    F32 phase = 0;
    //DWORD flags;
    BYTE* buffer;
    
    U32 bufferFrames, padding;
    IAudioClient_GetBufferSize(renderClient, &bufferFrames);
    
    while (true) 
    {
        DWORD wait = WaitForSingleObject(audio_event, INFINITE);
        
        if (wait != WAIT_OBJECT_0)
        {
            break;
        }
        
        IAudioClient_GetCurrentPadding(renderClient, &padding);
        U32 nFrames = bufferFrames - padding;
        
        if (nFrames == 0)
        {
            continue;
        }
        
        hr = IAudioRenderClient_GetBuffer(renderService, nFrames, &buffer);
        if (!SUCCEEDED(hr))
    	{ 
            continue;
        }
        
        gen_sign_wave_buff(440.0f,
                           (F32*)buffer, 
                           &phase,
                           format->nChannels,
                           nFrames, 
                           (F32)format->nSamplesPerSec);
        
        
        hr = IAudioRenderClient_ReleaseBuffer(renderService, nFrames, 0);
    }
    
    return 0;
}


void 
audio_thread_start()
{
    HANDLE thread = CreateThread(NULL,
                                 0,
                                 audio_thread,
                                 NULL,
                                 0,
                                 NULL
                                 );
    
}

void
audio_release()
{
    IAudioRenderClient_Release(renderService);
    
    IAudioClient_Stop(renderClient);
    IAudioClient_Release(renderClient);
    
    IMMDevice_Release(renderer);
    
    CoUninitialize();
}