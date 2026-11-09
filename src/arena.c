#include <stdio.h>
#include <assert.h>

#include "arena.h"
#include "types.h"
#include "defs.h"
#include "platform.h"
#include "str.h"
#include "lane.h"

#ifdef _WIN32
#include <windows.h>

static void* 
pages_reserve(U64 size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
}

static void 
pages_release(void* ptr, U64 size)
{
    VirtualFree(ptr, size, MEM_RELEASE);
}

static void*
pages_commit(void* ptr, U64 current_commited_bytes, U64 new_bytes_to_commit)
{
    return VirtualAlloc( (U8*)ptr + current_commited_bytes, new_bytes_to_commit, MEM_COMMIT, PAGE_READWRITE);
}

#else
#define _GNU_SOURCE
#include <sys/mman.h>

static void* 
page_alloc(U64 size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static void 
page_free(void* ptr, U64 size)
{
    munmap(ptr, size);
}
#endif

local_function U64
round_up_to_page(U64 memory_size)
{
    return (memory_size + (PAGE_SIZE - 1)) & ~(U64)(PAGE_SIZE-1);
}

local_function U64
required_pages(U64 memory_size)
{
    U64 page_number = memory_size / PAGE_SIZE;
    if (memory_size % PAGE_SIZE != 0)
    {
        page_number += 1;
    }
    return page_number;
}


void 
arena_create(Arena* a, U64 capacity)
{
    assert(capacity % PAGE_SIZE == 0);
    a->capacity = capacity;
    a->data = pages_reserve(a->capacity);
}

void 
arena_release(Arena* a) 
{
    pages_release(a->data, a->size);
}

RawPtr
arena_push(Arena* a, U64 size_of_new)
{
    U64 required = a->ptr + size_of_new;
    if ( required > a->size)
    {
        assert(required <= GIGABYTES(4));
        
        U64 size_to_commit = round_up_to_page(required - a->size);
        RawPtr res = pages_commit(a->data, a->size, size_to_commit);
        assert(res);
        a->size += size_to_commit;
    }
    void* ret = a->data + a->ptr;
    a->ptr += size_of_new;
    return ret;
}

RawPtr
arena_push_zero(Arena* a, U64 size_of_new)
{
    RawPtr p = arena_push(a, size_of_new);
    ZeroMemory(p, size_of_new);
    return p;
}

String8*
arena_dmp_str(Arena*a, const char* c_str)
{
    String8* res = push_struct(a, String8);
    res->size = cstr_len(c_str);
    res->data = arena_push_zero(a, res->size);
    CopyMemory(res->data, c_str, res->size);
    return res;
}


void
arena_pop(Arena* a, U64 size)
{
    if (a->ptr < size)
    {
        a->ptr = 0;
    }
    else 
    {
        a->ptr -= size;
    }
}

U64
arena_get_pos(Arena* a)
{
    return a->ptr;
}

void 
arena_set_pos(Arena* a, U64 pos)
{
    a->ptr = pos;
}

void 
arena_clear(Arena* a) 
{
    a->ptr = 0;
}

void 
arena_print(Arena* a)
{
    printf("Arena size: %zu, ptr: %lld\n", a->size, a->ptr);
    
    for(U64 i = 0; i < a->size; i+=16) {
        printf("%04zX: ", i);
        
        for(U64 j = 0; j < 16; ++j) {
            if( i + j == a-> ptr ) {
                printf(ANSI_COLOR_RED "%02X " ANSI_COLOR_RESET, a->data[i + j] );
            }
            else {
                printf("%02X ", a->data[i + j] );
            }
        }
        
        printf(" | ");
        
        for( int j = 0; j < 16; ++j) {
            if( is_ascii(a->data[i+j]) ) {
                printf("%c", a->data[i + j] );
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}

ArenaTemp 
arena_temp_begin(Arena* a)
{
    ArenaTemp temp = {0};
    temp.arena = a;
    temp.pos = a->ptr;
    return temp;
}

void 
arena_temp_end(ArenaTemp temp)
{
    arena_set_pos(temp.arena, temp.pos);
}

// TODO: addapt this such that it can have multiple scratches in each context and stop conflict between them
//ArenaTemp GetScratch(Arena **conflicts, U64 conflict_count); // grabs a thread-local scratch arena

ArenaTemp 
get_scratch()
{
    ArenaTemp temp = {0};
    temp.arena = &context.arena;
    temp.pos = context.arena.ptr;
    return temp;
}

