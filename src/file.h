/* date = November 8th 2026 10:35 pm */

#ifndef FILE_H
#define FILE_H

#include "types.h"
#include "str.h"

typedef struct 
{
    U64 size;
    U8* data;
} File;

File* file_load(Arena* a, String8 path);

U64 file_get_size(String8 path);



#endif //FILE_H
