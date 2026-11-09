#ifndef STR_H
#define STR_H

#include "types.h"

struct String8 
{
    U8*   data;
    U64   size;
};

String8 string8_new(char* c);
char* string8_to_char(Arena* arena, String8 str);
S32 string8_compare(String8 str1, String8 str2);

U64 cstr_len(const char* c_str);
bool is_ascii(char c);
#endif
