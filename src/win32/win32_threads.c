
internal W32_Barrier* 
w32_barrier_from_handle(Barrier handle)
{
    W32_Barrier* barrier = (W32_Barrier*)handle.u64[0];
    return barrier;
}



internal Barrier
barrier_create(U64 count) 
{
    Barrier handle = {0};
    if (count == 0) return handle;
    
    handle.u64[0] = IntFromPtr(push_struct(w32_state.arena, W32_Barrier));
    W32_Barrier* barrier = w32_barrier_from_handle(handle);
    
    InitializeSynchronizationBarrier( &barrier->impl, (LONG)count, -1);
}

internal void
barrier_wait(Barrier handle)
{
    W32_Barrier* barrier = w32_barrier_from_handle(handle);
    EnterSynchronizationBarrier(&barrier->impl, 0);
}

internal void
barrier_release(Barrier handle)
{
    W32_Barrier* barrier = w32_barrier_from_handle(handle);
    DeleteSynchronizationBarrier(&barrier->impl);
}
