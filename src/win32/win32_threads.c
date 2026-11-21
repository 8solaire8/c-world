internal Barrier 
barrier_create(U64 count) 
{
    Barrier barrier = {0};
    if (count == 0) return barrier;
    return InitializeSynchronizationBarrier(&b->impl, (LONG)count, -1) ? 0 : -1;
}

internal void
barrier_wait(Barrier b) 
{
    return EnterSynchronizationBarrier(&b->impl, 0) ? 1 : 0;
}

internal void
barrier_release(Barrier *b) 
{
    DeleteSynchronizationBarrier(&b->impl);
}
,