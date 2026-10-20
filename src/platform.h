/* date = October 18th 2026 11:25 am */

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


// TODO: make this opac to work on any
//typedef struct Window Window;
typedef struct
{
    HWND hwnd;
    HINSTANCE instance;
    int show;
} Window;

void create_window_win32(Window* window, HINSTANCE instance, int show);

void get_window_size(const Window* window, f32* width, f32* height);

bool start_window(Window* window, const wchar_t* name, u16 width, u16 hight);


#if defined(_WIN32)
#include <windows.h>

typedef struct 
{ 
    SYNCHRONIZATION_BARRIER impl; 
} Barrier;

static inline i32 
barrier_init(Barrier *b, unsigned count) 
{
    if (count == 0) return -1;
    return InitializeSynchronizationBarrier(&b->impl, (LONG)count, -1) ? 0 : -1;
}

static inline i32 
barrier_wait(Barrier *b) 
{
    return EnterSynchronizationBarrier(&b->impl, 0) ? 1 : 0;
}

static inline void 
barrier_destroy(Barrier *b) 
{
    DeleteSynchronizationBarrier(&b->impl);
}

#elif defined(__linux__)
#include <pthread.h>

typedef struct 
{ 
    pthread_Barrier impl;
} Barrier;

static inline i32 
barrier_init(Barrier *b, unsigned count) 
{
    if (count == 0) return -1;
    return pthread_barrier_init(&b->impl, NULL, count) == 0 ? 0 : -1;
}

static inline i32 
barrier_wait(Barrier *b) 
{
    int rc = pthread_barrier_wait(&b->impl);
    if (rc == PTHREAD_BARRIER_SERIAL_THREAD) return 1;
    return rc == 0 ? 0 : -1;
}

static inline void 
barrier_destroy(Barrier *b) 
{
    pthread_barrier_destroy(&b->impl);
}

#else
#error "barrier.h supports only Windows and Linux"
#endif

#endif //PLATFORM_H
