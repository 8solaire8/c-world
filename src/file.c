#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "file.h"
#include "arena.h"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

U64 file_get_size(String8 path)
{
    ArenaTemp temp = get_scratch();
    
    wchar* wide = string8_to_wchar(temp.arena, path);
    U64 res = 0;
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (GetFileAttributesExW(wide, GetFileExInfoStandard, &data))
    {
        res = ((uint64_t)data.nFileSizeHigh << 32) | data.nFileSizeLow;
    }
    release_scratch(temp);
    return res; 
}
#elif defined(__linux__)
#include <sys/stat.h>

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
#else 
U64
file_get_size(String8 path)
{
    ArenaTemp temp = get_scratch();
    char* c_path = string8_to_char(temp.arena, path);
    FILE* file = fopen(c_path, "rb");
    fseek(file, 0, SEEK_END);
    U64 size = ftell(file);
    fclose(file);
    release_scratch(temp);
    return size;
}
#endif

File*
file_load(Arena* arena, String8 path)
{
    ArenaTemp temp = get_scratch();
    File* file = push_struct_zero(arena, File);
    file->size = file_get_size(path);
    char* c_path = string8_to_char(temp.arena, path);
    FILE* fd = fopen(c_path, "rb");
    fread(file->data, file->size, 1, fd);
    fclose(fd);
    release_scratch(temp);
    return file;
}