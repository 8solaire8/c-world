#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define local_function static 

typedef void*         RawPtr;
typedef float         F32;
typedef double        F64;
typedef uint8_t       U8;
typedef uint16_t      U16;
typedef uint32_t      U32;
typedef uint64_t      U64;
typedef int8_t        S8;
typedef int16_t       S16;
typedef int32_t       S32;
typedef int64_t       S64;

typedef struct Arena Arena;
typedef struct ArenaTemp ArenaTemp;
typedef struct String8 String8;
typedef struct Context Context;
typedef struct LaneRange LaneRange;
typedef struct Window Window;
typedef struct Barrier Barrier;
typedef struct Colour Colour;
#endif