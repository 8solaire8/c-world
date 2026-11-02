#define COBJMACROS 
#include <stdio.h>
#include <Windows.h>
#include <initguid.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>
#include <assert.h>
#include <math.h>

#include "audio.h"
#include "defs.h"

#pragma comment(lib, "ole32.lib")


const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E,0x3D,0xC4,0x57,0x92,0x91,0x69,0x2E}};
const IID   IID_IMMDeviceEnumerator  = {0xA95664D2, 0x9614, 0x4F35, {0xA7,0x46,0xDE,0x8D,0xB6,0x36,0x17,0xE6}};
const IID   IID_IAudioClient         = {0x1CB9AD4C, 0xDBFA, 0x4C32, {0xB1,0x78,0xC2,0xF5,0x68,0xA7,0x03,0xB2}};
const IID   IID_IAudioRenderClient   = {0xF294ACFC, 0x3146, 0x4483, {0xA7,0xBF,0xAD,0xDC,0xA7,0xC2,0x60,0xE2}};



static IMMDeviceEnumerator* enumerator = NULL;
static IMMDevice* renderer = NULL;
static IAudioClient* renderClient = NULL;
static IAudioRenderClient* renderService = NULL;

static WAVEFORMATEX* format = NULL;


// TO DO
// make event driven 
// have it work if sound source changes

bool
audio_init() 
{
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
    if(!SUCEEDED(hr)) return false;
    
    IMMDeviceEnumerator_Release(enumerator);
    
    if(!SUCEEDED(hr)) return false;
    
    hr = IMMDevice_Activate(renderer, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&renderClient);
    
    if(!SUCEEDED(hr)) return false;
    
    hr = IAudioClient_GetMixFormat(renderClient, &format);
    
    if(!SUCEEDED(hr)) return false;
    
    hr = IAudioClient_Initialize(renderClient, AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, format, NULL);
    
    if(!SUCEEDED(hr)) return false;
    
    hr = IAudioClient_GetService(renderClient, &IID_IAudioRenderClient, (void**)&renderService);
    
    if(!SUCEEDED(hr)) return false;
}

void 
audio_thread()
{
    hr = IAudioClient_Start(renderClient);
    
    if(!SUCCEEDED(hr))
    {
        return false;
    }
    
    F64 phase = 0;
    DWORD flags;
    BYTE* buffer;
    
    U32 bufferFrames, padding;
    IAudioClient_GetBufferSize(renderClient, &bufferFrames);
    
    while (true) 
    {
        IAudioClient_GetCurrentPadding(renderClient, &padding);
        UINT32 nFrames = bufferFrames - padding;
        
        if (nFrames == 0)
        {
            continue;
        }
        
        
        hr = IAudioRenderClient_GetBuffer(renderService, nFrames, &buffer);
        
        if (!SUCCEEDED(hr))
    	{ 
            continue;
        }
        
        F32* samples = (F32*)buffer;
        U32 channels = (U32)format->nChannels;
        
        for (U64 i = 0; i < nFrames; ++i) 
        {
            F32 sample = sinf(phase) * 0.1f;
            
            for (int c = 0; c < channels; ++c ) 
            {
                samples[i * channels + c] = sample;
            }
            
            phase += (M_TAU * 440.0) / (F64)(format->nSamplesPerSec);
            
            if (phase >= M_TAU) 
            {
                phase -= M_TAU;
            }
        }
        
        hr = IAudioRenderClient_ReleaseBuffer(renderService, nFrames, 0);
    }
    
}


void
audio_free()
{
    IAudioRenderClient_Release(renderService);
    
    IAudioClient_Stop(renderClient);
    IAudioClient_Release(renderClient);
    
    IMMDevice_Release(renderer);
    
    CoUninitialize();
}