#ifndef BASE_THREADS_H
#define BASE_THREADS_H


typedef struct Barrier Barrier;
struct Barrier
{
    U64 u64[1];
};

internal Barrier barrier_create(U64 count);
internal void barrier_wait(Barrier handle);
internal void barrier_release(Barrier handle);

#endif //BASE_THREADS_H
