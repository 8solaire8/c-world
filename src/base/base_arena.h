#ifndef BASE_ARENA_H
#define BASE_ARENA_H

typedef struct Arena Arena;
struct Arena
{
    U64 ptr;
    U64 size;
    U64 capacity;
    U8 *data;
    U64 base_pos;
};

typedef struct ArenaTemp ArenaTemp;
struct ArenaTemp
{
    Arena *arena;
    U64 pos;
};

internal Arena *arena_create();
internal Arena *arena_create_sized(U64 capacity);
internal void arena_release(Arena *a);

internal RawPtr arena_push(Arena *a, U64 size);
internal RawPtr arena_push_set(Arena *a, U64 size, U8 set_val);

#define push_array(arena, type, count)(type*) arena_push((arena), sizeof(type)*(count))
#define push_array_zero(arena, type, count)(type*)arena_push_set((arena), sizeof(type)*(count), 0)

#define push_array_no_zero(arena, type, count)(type*)arena_push_set((arena), sizeof(type)*(count), 1)
#define push_struct(arena, type) push_array((arena), type, 1)
#define push_struct_zero(arena, type) push_array_zero((arena), type, 1)

internal void arena_pop(Arena *a, U64 size);
internal U64 arena_get_pos(Arena* a);

internal void arena_set_pos(Arena *a, U64 pos);
internal void arena_clear(Arena *a);

internal void arena_print(Arena *a);


internal ArenaTemp arena_temp_begin(Arena *a);
internal void arena_temp_end(ArenaTemp temp);


#endif //BASE_ARENA_H