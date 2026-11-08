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

File file_open(String8 name);
void file_size(const File* name);
void file_size_path(String8* name);



#endif //FILE_H
