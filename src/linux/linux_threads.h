#ifndef LINUX_THREADS_H
#define LINUX_THREADS_H

typedef struct Lnx_Barrier Lnx_Barrier;
struct Lnx_Barrier
{ 
    pthread_Barrier impl;
};

#endif //LINUX_THREADS_H
