#ifndef ARENA_H_
#define ARENA_H_

#include "types.h"

typedef struct 
{
    U8* data;
    U64 ptr;
    U64 size;
}Arena;

void arena_create(Arena* a, U64 size);

void* arena_alloc(Arena* a, U64 size );
void* arena_relloc(Arena* a, void* old_data, U64 old_size, U64 new_size);

void arena_clear(Arena* a);
void arena_free(Arena* a); 

void arena_print(Arena* a);


#endif