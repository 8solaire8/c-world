#ifndef STRINGS_H_
#define STRINGS_H_

#include "arena.h"

typedef struct {
    char* data;
    usize size;
} String;


String FromChar(Arena* a, char* c);
char* ToChar(String* str);

#endif
