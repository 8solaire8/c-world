
#ifdef _WIN32
#include <windows.h>

static void* 
PageAlloc(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void*
PageReAlloc(void* ptr, size_t oldSize, size_t newSize)
{
}

static void 
PageFree(void* ptr, size_t size)
{
    VirtualFree(ptr, size, MEM_RESERVE);
}

#else
#define _GNU_SOURCE
#include <sys/mman.h>

static void* 
PageAlloc(size_t size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

static void*
PageReAlloc(void* ptr, size_t oldSize, size_t newSize)
{
    a->data = mremap(a->data, original_size, a->size, MREMAP_MAYMOVE);
}

static void 
PageFree(void* ptr, size_t size) 
{
    munmap(ptr, size);
}
#endif


#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#include "arena.h"
#include "types.h"


static size_t round_up_to_page(size_t size) {
    return (size + 4095) & ~(size_t)4095;
}


void ArenaCreate(Arena* a, size_t size) {
    a->size = round_up_to_page(size);
    a->data = PageAlloc(a->size);
    a->ptr  = 0;
}

void* ArenaAlloc(Arena* a, size_t size ) { 
    if(a->ptr + size > a->size) {
        size_t original_size = a->size;
        while ( a->ptr + size > a->size ) {
            a->size *= 2;
        }
        a->size = round_up_to_page(a->size);
        a->data = PageReAlloc(a->data, original_size, a->size);
    }
    void* ret = a->data + a->ptr;
    a->ptr += size;
    return ret;
}

void* ArenaRelloc(Arena* a, void* old_data, size_t old_size, size_t new_size) { 
    void* new_data = ArenaAlloc(a, new_size);
    memcpy(new_data, old_data, old_size );
    return new_data;
}

void ArenaClear(Arena* a) {
    a->ptr = 0;
}

void ArenaFree(Arena* a) {
    PageFree(a->data, a->size);
}

void ArenaPrint(Arena* a) {
    printf("Arena size: %d, ptr: %d\n", a->size, a->ptr);
    
    for( int i = 0; i < a->size; i+=16) {
        printf("%04zX: ", i);
        
        for( int j = 0; j < 16; ++j) {
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



