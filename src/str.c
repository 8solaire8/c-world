
#include "str.h"
#include "Arena.h"

String8 
string8_new(char* c_string) 
{
    String8 res = {0};
    U64 size = strlen(c_string);
    res.data = c_string;
    res.size = size;
    return res;
}

char*
string8_to_char(Arena* arena, const String8* str)
{
    char* res = arena_alloc(arena, str->size);
    memcpy(res, str->data, str->size);
    return res;
}


