#ifndef ARENA_H_
#define ARENA_H_

#include "types.h"

typedef struct 
{
    U8* data;
    U64 ptr;
    U64 size;
    U64 capacity;
}Arena;

typedef struct 
{
    Arena* arena;
    U64 pos;
} ArenaTemp;


void arena_alloc(Arena* a, U64 capacity);
void arena_release(Arena* a);

RawPtr arena_push(Arena* a, U64 size);
RawPtr arena_push_zero(Arena* a, U64 size);


#define push_array(arena, type, count)(type*) arena_push((arena), sizeof(type)*(count))
#define push_array_zero(arena, type, count)(type*)arena_push_zero((arena), sizeof(type)*(count))
#define push_struct(arena, type) push_array((arena), type, 1)
#define push_struct_zero(arena, type) push_array_zero((arena), type, 1)

void arena_pop(Arena* a, U64 size);
U64 arena_get_pos(Arena* a);

void arena_set_pos(Arena* a, U64 pos);
void arena_clear(Arena* a);

void arena_print(Arena* a);


ArenaTemp arena_temp_begin(Arena* a);
void arena_temp_end(ArenaTemp temp);

void init_scratch();
ArenaTemp get_scratch();
#define release_scratch(t) arena_temp_end(t)

#endif