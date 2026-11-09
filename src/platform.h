#ifndef PLATFORM_H
#define PLATFORM_H

#include "types.h"

#ifndef thread_local
#ifdef _WIN32
#include <windows.h>
#define thread_local __declspec(thread)
#elif defined(__linux__)
#define thread_local __thread
#else
#error platform not supported
#endif
#endif

//TODO: make this an opac type that works on linux
struct Window
{
    HWND hwnd;
    HINSTANCE instance;
    S32 show;
};

void create_window_win32(Window* window, HINSTANCE instance, int show);

void get_window_size(const Window* window, F32* width, F32* height);

bool start_window(Window* window, const wchar_t* name, U16 width, U16 hight);


#if defined(_WIN32)
#include <windows.h>

struct Barrier
{ 
    SYNCHRONIZATION_BARRIER impl; 
};

static inline S32 
barrier_create(Barrier *b, unsigned count) 
{
    if (count == 0) return -1;
    return InitializeSynchronizationBarrier(&b->impl, (LONG)count, -1) ? 0 : -1;
}

static inline S32 
barrier_wait(Barrier *b) 
{
    return EnterSynchronizationBarrier(&b->impl, 0) ? 1 : 0;
}

static inline void 
barrier_release(Barrier *b) 
{
    DeleteSynchronizationBarrier(&b->impl);
}

#elif defined(__linux__)
#include <pthread.h>

struct Barrier
{ 
    pthread_Barrier impl;
};

static inline S32 
barrier_create(Barrier *b, unsigned count) 
{
    if (count == 0) return -1;
    return pthread_barrier_init(&b->impl, NULL, count) == 0 ? 0 : -1;
}

static inline S32 
barrier_wait(Barrier *b) 
{
    int rc = pthread_barrier_wait(&b->impl);
    if (rc == PTHREAD_BARRIER_SERIAL_THREAD) return 1;
    return rc == 0 ? 0 : -1;
}

static inline void 
barrier_release(Barrier *b) 
{
    pthread_barrier_destroy(&b->impl);
}

#else
#error "barrier.h supports only Windows and Linux"
#endif

#endif //PLATFORM_H
