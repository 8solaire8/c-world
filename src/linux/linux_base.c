
 internal void* 
page_alloc(U64 size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

internal void 
page_free(void* ptr, U64 size)
{
    munmap(ptr, size);
}


U64 get_file_size(String8 path)
{
    ArenaTemp temp = get_scratch();
    char* c_path = string8_to_char(temp.arena, path);
    U64 res = 0;
    struct stat st;
    
    if (stat(path, &st) != 0 || st.st_size < 0)
    {
        res = 0;
    }
    else 
    {
        res = (U64)st.st_size;
    }
    release_scratch(temp);
    return res;
}
