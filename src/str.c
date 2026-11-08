#include <string.h>

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
    char* res = arena_push(arena, str->size);
    memcpy(res, str->data, str->size);
    return res;
}

S32 
compare_strings(const char* str1, const char* str2) 
{
    if (str1 == NULL && str2 == NULL) 
    {
        return 0; 
    }
    if (str1 == NULL) 
    {
        return -1;
    }
    if (str2 == NULL) 
    {
        return 1; 
    }
    return strcmp(str1, str2); 
}

