
internal Barrier 
barrier_create(U64 count) 
{
    Barrier barrier = {0};
    if (count == 0) return barrier;
    
    return pthread_barrier_init(&b->impl, NULL, count) == 0 ? 0 : -1;
}

internal void
barrier_wait(Barrier barrier) 
{
    Lnx_Barrier *lnx_barrier = (Lnx_Barrier*)PtrFromInt(barrier.u64[0]);
    int rc = pthread_barrier_wait(&lnx_barrier->impl);
    //if (rc == PTHREAD_BARRIER_SERIAL_THREAD);
}

internal void 
barrier_release(Barrier barrier) 
{
    Lnx_Barrier *lnx_barrier = (Lnx_Barrier*)PtrFromInt(barrier.u64[0]);
    pthread_barrier_destroy(&lnx_barrier->impl);
}
