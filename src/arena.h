#ifndef ARENA_H_
#define ARENA_H_

#include "types.h"

typedef struct 
{
    byte* data;
    u32   ptr;
    usize size;
}Arena;

void ArenaCreate(Arena* a, usize size);

void* arena_alloc(Arena* a, usize size );
void* arena_relloc(Arena* a, void* old_data, usize old_size, usize new_size);

void arena_clear(Arena* a);
void arena_free(Arena* a); 

void arena_print(Arena* a);


#endif