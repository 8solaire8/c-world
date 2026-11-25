internal void*
pages_reserve(U64 size)
{
    void* ptr = mmap(NULL, size, PROT_NONE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    return (ptr == MAP_FAILED) ? NULL : ptr;
}

internal void
pages_release(void* ptr, U64 size)
{
    munmap(ptr, size);
}

internal void*
pages_commit(void* ptr, U64 current_commited_bytes, U64 new_bytes_to_commit)
{
    U64 ps    = (U64)sysconf(_SC_PAGESIZE);
    U64 addr  = (U64)ptr + current_commited_bytes;
    U64 start = addr & ~(ps - 1);
    U64 end   = (addr + new_bytes_to_commit + ps - 1) & ~(ps - 1);
    
    if (mprotect((void*)start, end - start, PROT_READ | PROT_WRITE) != 0) { return NULL; }
    return (void*)start;
}

U64 get_file_size(String8 path)
{
    ArenaTemp temp = scratch_begin();
    char* c_path = str8_to_char(temp.arena, path);
    U64 res = 0;
    struct stat st;
    if (stat(c_path, &st) != 0 || st.st_size < 0)
    {
        res = 0;
    }
    else 
    {
        res = (U64)st.st_size;
    }
    scratch_end(temp);
    return res;
}
