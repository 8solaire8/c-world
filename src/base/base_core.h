#ifndef BASE_CORE_H
#define BASE_CORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <wchar.h>

typedef wchar_t       wchar;
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


#if COMPILER_MSVC || (COMPILER_CLANG && OS_WINDOWS)
# pragma section(".rdata$", read)
# define read_only __declspec(allocate(".rdata$"))
#elif (COMPILER_CLANG && OS_LINUX)
# define read_only __attribute__((section(".rodata")))
#else
# define read_only
#endif


#define internal      static
#define global        static
#define local         static

#if COMPILER_MSVC
# define thread_static __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
# define thread_static __thread
#else
# error thread_static not defined for this compiler.
#endif

#define IntFromPtr(ptr) ((U64)(ptr))
#define PtrFromInt(i) (void*)(i)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define M_TAU (2*M_PI)

#define PAGE_SIZE 4096

#define KILOBYTES(x) ((U64)(x)     * 1024ULL)
#define MEGABYTES(x) (KILOBYTES(x) * 1024ULL)
#define GIGABYTES(x) (MEGABYTES(x) * 1024ULL)
#define PAGES(x) (PAGE_SIZE * x)

#define UNUSED(x) ((void)(x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

internal void os_create_subsystem();

#endif