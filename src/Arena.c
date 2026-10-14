
#ifdef _WIN32
#include <windows.h>

static void* 
page_alloc(usize size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void 
page_free(void* ptr, usize size)
{
    VirtualFree(ptr, size, MEM_RESERVE);
}

#else
#define _GNU_SOURCE
#include <sys/mman.h>

static void* 
page_alloc(usize size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static void 
page_free(void* ptr, usize size) 
{
    munmap(ptr, size);
}
#endif


#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#include "arena.h"
#include "types.h"


static usize round_up_to_page(usize size) {
    return (size + 4095) & ~(usize)4095;
}


void arena_create(Arena* a, usize size) {
    a->size = round_up_to_page(size);
    a->data = page_alloc(a->size);
    a->ptr  = 0;
}

void* arena_alloc(Arena* a, usize size ) { 
    assert(a->ptr + size < a->size);
    void* ret = a->data + a->ptr;
    a->ptr += size;
    return ret;
}

void* arena_relloc(Arena* a, void* old_data, usize old_size, usize new_size) { 
    void* new_data = arena_alloc(a, new_size);
    memcpy(new_data, old_data, old_size );
    return new_data;
}

void arena_clear(Arena* a) {
    a->ptr = 0;
}

void arena_free(Arena* a) {
    page_free(a->data, a->size);
}

void arena_print(Arena* a) {
    printf("Arena size: %zu, ptr: %d\n", a->size, a->ptr);
    
    for( usize i = 0; i < a->size; i+=16) {
        printf("%04zX: ", i);
        
        for( usize j = 0; j < 16; ++j) {
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
