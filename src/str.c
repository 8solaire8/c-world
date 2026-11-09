#include <windows.h>

#include "defs.h"
#include "str.h"
#include "arena.h"

String8 
string8_new(char* c_string) 
{
    String8 res = {0};
    U64 size = cstr_len(c_string);
    res.data = c_string;
    res.size = size;
    return res;
}

char*
string8_to_char(Arena* arena, String8 str)
{
    char* res = arena_push(arena, str.size);
    CopyMemory(res, str.data, str.size);
    return res;
}



S32 
string8_compare(String8 a, String8 b) 
{
    U64 min_size = MIN(a.size, b.size);
    
    for (U64 i = 0; i < min_size; ++i) 
    {
        if (a.data[i] != b.data[i]) return (S32)a.data[i] - (S32)b.data[i];
    }
    
    if (a.size < b.size) return -(S32)(b.size - a.size);
    if (a.size > b.size) return  (S32)(a.size - b.size);
    return 0;
}


U64 
cstr_len(const char* c_str)
{
    U64 length = 0;
    while(c_str[length] != '\0')
    {
        length++;
    }
    return length;
}

bool 
is_ascii(char c)
{
    return c>= 0x20 && c <= 0x7E;
}
