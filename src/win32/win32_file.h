#ifndef WIN32_FILE_H
#define WIN32_FILE_H

typedef struct 
{
    U64 size;
    U8* data;
} File;

File* file_load(Arena* a, String8 path);

U64 file_get_size(String8 path);

#endif //WIN32_FILE_H
