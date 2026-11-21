#include <stdio.h>
#include <stdlib.h>



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

File*
file_load(Arena* arena, String8 path)
{
    ArenaTemp temp = scratch_begin();
    File* file = push_struct_zero(arena, File);
    file->size = file_get_size(path);
    char* c_path = string8_to_char(temp.arena, path);
    FILE* fd = fopen(c_path, "rb");
    fread(file->data, file->size, 1, fd);
    fclose(fd);
    scratch_end(temp);
    return file;
}