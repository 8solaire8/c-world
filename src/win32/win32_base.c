internal void* 
pages_reserve(U64 size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
}

internal void 
pages_release(void* ptr, U64 size)
{
    VirtualFree(ptr, size, MEM_RELEASE);
}

internal void*
pages_commit(void* ptr, U64 current_commited_bytes, U64 new_bytes_to_commit)
{
    return VirtualAlloc( (U8*)ptr + current_commited_bytes, new_bytes_to_commit, MEM_COMMIT, PAGE_READWRITE);
}

internal void
os_create_subsystem()
{
    w32_state.arena = arena_create();
}