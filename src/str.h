#ifndef STRINGS_H_
#define STRINGS_H_

#include "arena.h"

typedef struct 
{
    U8*   data;
    U64   size;
} String8;


String8 string8_new(char* c);
char* string8_to_char(Arena* arena, const String8* str);

#endif
