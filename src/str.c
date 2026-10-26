
#include "string.h"

String 
from_char(Arena* a,char* c) 
{
    String res = {0};
    size_t size = strlen(c);
    res.data = arena_alloc(a, size + 1);
    res.data[size] = '\0';
    memcpy(res.data, c, size);
    res.size = size;
    return res;
}

char* 
to_char(String* str)
{
    return str->data;
}


