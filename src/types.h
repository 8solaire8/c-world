#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include "arena.h"

typedef unsigned char byte;
typedef float         f32;
typedef double        f64;
typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef uint64_t      u64;
typedef int8_t        i8;
typedef int16_t       i16;
typedef int32_t       i32;
typedef int64_t       i64;
typedef size_t        usize;


typedef struct {
    char* data;
    usize size;
} String;


//String FromChar(Arena* a, char* c);
//char* ToChar(String* str);

#endif


