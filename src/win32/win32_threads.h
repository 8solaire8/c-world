#ifndef WIN32_THREADS_H
#define WIN32_THREADS_H

typedef struct W32_Barrier W32_Barrier; 
struct W32_Barrier
{ 
    SYNCHRONIZATION_BARRIER impl; 
};

internal W32_Barrier* w32_barrier_from_handle(Barrier handle);

#endif //WIN32_THREADS_H
