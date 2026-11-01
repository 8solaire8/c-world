#define COBJMACROS 
#include <stdio.h>
#include <Windows.h>
#include <initguid.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#pragma comment(lib, "ole32.lib")


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define M_TAU (2*M_PI)

const CLSID CLSID_MMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E,0x3D,0xC4,0x57,0x92,0x91,0x69,0x2E}};
const IID   IID_IMMDeviceEnumerator  = {0xA95664D2, 0x9614, 0x4F35, {0xA7,0x46,0xDE,0x8D,0xB6,0x36,0x17,0xE6}};
const IID   IID_IAudioClient         = {0x1CB9AD4C, 0xDBFA, 0x4C32, {0xB1,0x78,0xC2,0xF5,0x68,0xA7,0x03,0xB2}};
const IID   IID_IAudioRenderClient   = {0xF294ACFC, 0x3146, 0x4483, {0xA7,0xBF,0xAD,0xDC,0xA7,0xC2,0x60,0xE2}};

static IMMDeviceEnumerator* enumerator = NULL;
static IMMDevice* renderer = NULL;
static IAudioClient* renderClient = NULL;
static IAudioRenderClient* renderService = NULL;

static WAVEFORMATEX* format = NULL;


int
main() 
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
    assert(SUCCEEDED(hr));
    
    
    IMMDeviceEnumerator_Release(enumerator);
    
    assert(SUCCEEDED(hr));
    
    hr = IMMDevice_Activate(renderer, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&renderClient);
    
    assert(SUCCEEDED(hr));
    
    hr = IAudioClient_GetMixFormat(renderClient, &format);
    
    assert(SUCCEEDED(hr));
    
    printf("Mix format:\n");
    printf("  Frame size     : %d\n", format->nBlockAlign);
    printf("  Channels       : %d\n", format->nChannels);
    printf("  Bits per second: %d\n", format->wBitsPerSample);
    printf("  Sample rate:   : %d\n", format->nSamplesPerSec);
    
    
    hr = IAudioClient_Initialize(renderClient, AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, format, NULL);
    assert(SUCCEEDED(hr));
    
    hr = IAudioClient_GetService(renderClient, &IID_IAudioRenderClient, (void**)&renderService);
    assert(SUCCEEDED(hr));
    
    
    UINT32 nFrames;
    DWORD flags;
    BYTE* buffer;
    
    hr = IAudioClient_Start(renderClient);
    assert(SUCCEEDED(hr));
    
    double phase = 0;
    
    while (true) 
    {
        
        hr = IAudioRenderClient_GetBuffer(renderService, nFrames, &buffer);
        
        /*
TO DO 

UINT32 bufferFrames, padding;
IAudioClient_GetBufferSize(audioClient, &bufferFrames);   // once, after Initialize

// each time round the loop:
IAudioClient_GetCurrentPadding(audioClient, &padding);
UINT32 nFrames = bufferFrames - padding;
if (nFrames == 0) { // nothing to write }

*/
        
        if (SUCCEEDED(hr))
    	{ 
			float* samples = (float*)buffer;
            int channels = (int)format->nChannels;
            
            for (int i = 0; i < nFrames; ++i) 
			{
			    float sample = sinf(phase) * 0.1f;
                
                for (int c = 0; c < channels; ++c ) 
			    {
			        samples[i * channels + c] = sample;
			    }
                
			    phase += (M_TAU * 440.0) / (double)(format->nSamplesPerSec);
                
                if (phase >= M_TAU) 
			    {
			        phase -= M_TAU;
			    }
			}
            
            hr = IAudioRenderClient_ReleaseBuffer(renderService, nFrames, 0);
    	}
        
        
        
    }
    
    IAudioRenderClient_Release(renderService);
    
    IAudioClient_Stop(renderClient);
    IAudioClient_Release(renderClient);
    
    IMMDevice_Release(renderer);
    
    CoUninitialize();
}