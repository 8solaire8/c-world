#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include "types.h"
#include "defs.h"
#include "arena.h"



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
pages_commit(void* ptr, U64 commited, U64 size)
{
    return VirtualAlloc( (U8*)ptr + commited, size, MEM_COMMIT, PAGE_READWRITE);
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
        page_number + 1;
    }
    return page_number;
}


void 
arena_create(Arena* a, U64 size)
{
    assert(size % PAGE_SIZE == 0);
    pages_reserve(GIGABYTES(4));
    a->data = pages_commit(a->data, a->size, size);
    a->size = size;
    a->ptr  = 0;
}

void* 
arena_alloc(Arena* a, U64 size )
{ 
    if (a->ptr + size >= a->size)
    {
        assert(a->ptr + size < GIGABYTES(4));
        
        U64 new_size = round_up_to_page(size);
        pages_commit(a->data, a->size, new_size);
        a->size += new_size;
    }
    void* ret = a->data + a->ptr;
    a->ptr += size;
    return ret;
}

void* 
arena_relloc(Arena* a, void* old_data, U64 old_size, U64 new_size)
{ 
    void* new_data = arena_alloc(a, new_size);
    memcpy(new_data, old_data, old_size );
    return new_data;
}

void 
arena_clear(Arena* a) 
{
    a->ptr = 0;
}

void arena_free(Arena* a) 
{
    pages_release(a->data, a->size);
}

void arena_print(Arena* a)
{
    printf("Arena size: %zu, ptr: %d\n", a->size, a->ptr);
    
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
            if( isprint(a->data[i+j]) ) {
                printf("%c", a->data[i + j] );
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }
}
