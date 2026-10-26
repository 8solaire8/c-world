#ifndef STRINGS_H_
#define STRINGS_H_

#include "arena.h"

typedef struct 
{
    char* data;
    usize size;
} String;


String from_char(Arena* a, char* c);
char* to_char(String* str);

#endif
