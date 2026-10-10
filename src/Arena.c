#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int i32;
typedef unsigned char Byte;

typedef struct {
    Byte* data;
    i32 ptr;
    size_t size;
}Arena;

size_t round_up_to_page(size_t size) {
    return (size + 4095) & ~(size_t)4095;
}


void ArenaCreate(Arena* a, size_t size) {
    a->data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    a->ptr  = 0;
    a->size = round_up_to_page(size);

}

void* ArenaAlloc(Arena* a, size_t size ) { 
    if(a->ptr + size > a->size) {
        size_t original_size = a->size;
        while ( a->ptr + size > a->size ) {
            a->size *= 2;
        }
        a->size = round_up_to_page(a->size);
        a->data = mremap(a->data, original_size, a->size, MREMAP_MAYMOVE);
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
    munmap(a->data, a->size);
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


typedef struct {
    char* data;
    size_t size;
} String;


String FromChar(Arena* a,char* c) {
    String res = {0};
    size_t size = strlen(c);
    res.data = ArenaAlloc(a, size + 1);
    res.data[size] = '\0';
    memcpy(res.data, c, size);
    res.size = size;
    return res;
}

char* ToChar(String* str){
    return str->data;
}


int main(i32 argc, char* argv[]){
    
    Arena a = {0};
    
    ArenaCreate(&a, 153);

    int* ar = ArenaAlloc(&a, sizeof(int)*10);
 
    for(int i = 0; i < 10; i++){
        ar[i] = i;
    }
    ar[0] = 3000;

    ArenaPrint(&a);
    
    ar = ArenaRelloc(&a, ar, sizeof(int) * 10, sizeof(int) * 15);

    String hello = FromChar(&a, "hello world");
    String hello2 = FromChar(&a, "hello world to the wowrd");
    
    printf("%s\n", hello2.data);

    ArenaPrint(&a);

    printf("\n");
    
    return 0;
}

