#ifndef ARENA_H_
#define ARENA_H_

#include "types.h"

typedef struct {
    byte* data;
    u32 ptr;
    size_t size;
}Arena;

void ArenaCreate(Arena* a, size_t size);

void* ArenaAlloc(Arena* a, size_t size );
void* ArenaRelloc(Arena* a, void* old_data, size_t old_size, size_t new_size);

void ArenaClear(Arena* a);
void ArenaFree(Arena* a); 

void ArenaPrint(Arena* a);




#endif
