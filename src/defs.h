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

#include "types.h"

#define PAGE_SIZE 4096

#define KILOBYTES(x) ((U64)(x) * 1024ULL)
#define MEGABYTES(x) (KILOBYTES(x) * 1024ULL)
#define GIGABYTES(x) (MEGABYTES(x) * 1024ULL)
#define PAGES(x) (PAGE_SIZE * x)

#if defined(_WIN32)
#include <windows.h>


typedef struct
{
    SYNCHRONIZATION_BARRIER impl;
} Barrier;

static inline S32
barrier_init(Barrier* b, unsigned count)
{
    if (count == 0) return -1;
    return InitializeSynchronizationBarrier(&b->impl, (LONG)count, -1) ? 0 : -1;
}

static inline S32
barrier_wait(Barrier* b)
{
    return EnterSynchronizationBarrier(&b->impl, 0) ? 1 : 0;
}

static inline void
barrier_destroy(Barrier* b)
{
    DeleteSynchronizationBarrier(&b->impl);
}

#elif defined(__linux__)
#include <pthread.h>

typedef struct
{
    pthread_Barrier impl;
} Barrier;

static inline S32
barrier_init(Barrier* b, unsigned count)
{
    if (count == 0) return -1;
    return pthread_barrier_init(&b->impl, NULL, count) == 0 ? 0 : -1;
}

static inline S32
barrier_wait(Barrier* b)
{
    S32 rc = pthread_barrier_wait(&b->impl);
    if (rc == PTHREAD_BARRIER_SERIAL_THREAD) return 1;
    return rc == 0 ? 0 : -1;
}

static inline void
barrier_destroy(Barrier* b)
{
    pthread_barrier_destroy(&b->impl);
}

#else
#error "barrier.h supports only Windows and Linux"
#endif