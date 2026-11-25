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

#if COMPILER_MSVC
# define Trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
# define Trap() __builtin_trap()
#else
# error Unknown trap intrinsic for this compiler.
#endif

#define AssertAlways(x) do{if(!(x)) {Trap();}}while(0)
#if BUILD_DEBUG
# define Assert(x) AssertAlways(x)
#else
# define Assert(x) (void)(x)
#endif
#define InvalidPath        AssertAlways(!"Invalid Path!")
#define NotImplemented     AssertAlways(!"Not Implemented!")
#define NoOp               ((void)0)
#define StaticAssert(C, ID) global U8 Glue(ID, __LINE__)[(C)?1:-1]

#define IntFromPtr(ptr) ((U64)(ptr))
#define PtrFromInt(i) (void*)(i)


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

global F32 pi32 = 3.1415926535897f;
global F32 tau32 = 6.2831853071795f;

global U64 max_U64 = 0xffffffffffffffffull;
global U32 max_U32 = 0xffffffff;
global U16 max_U16 = 0xffff;
global U8  max_U8  = 0xff;

global S64 max_S64 = (S64)0x7fffffffffffffffll;
global S32 max_S32 = (S32)0x7fffffff;
global S16 max_S16 = (S16)0x7fff;
global S8  max_S8  =  (S8)0x7f;

global S64 min_S64 = (S64)0x8000000000000000ll;
global S32 min_S32 = (S32)0x80000000;
global S16 min_S16 = (S16)0x8000;
global S8  min_S8  =  (S8)0x80;

internal void os_create_subsystem();

internal U16 safe_cast_u16(U32 x);
internal U32 safe_cast_u32(U64 x);
internal S32 safe_cast_s32(S64 x);


global const U32 bitmask1  = 0x00000001;
global const U32 bitmask2  = 0x00000003;
global const U32 bitmask3  = 0x00000007;
global const U32 bitmask4  = 0x0000000f;
global const U32 bitmask5  = 0x0000001f;
global const U32 bitmask6  = 0x0000003f;
global const U32 bitmask7  = 0x0000007f;
global const U32 bitmask8  = 0x000000ff;
global const U32 bitmask9  = 0x000001ff;
global const U32 bitmask10 = 0x000003ff;
global const U32 bitmask11 = 0x000007ff;
global const U32 bitmask12 = 0x00000fff;
global const U32 bitmask13 = 0x00001fff;
global const U32 bitmask14 = 0x00003fff;
global const U32 bitmask15 = 0x00007fff;
global const U32 bitmask16 = 0x0000ffff;
global const U32 bitmask17 = 0x0001ffff;
global const U32 bitmask18 = 0x0003ffff;
global const U32 bitmask19 = 0x0007ffff;
global const U32 bitmask20 = 0x000fffff;
global const U32 bitmask21 = 0x001fffff;
global const U32 bitmask22 = 0x003fffff;
global const U32 bitmask23 = 0x007fffff;
global const U32 bitmask24 = 0x00ffffff;
global const U32 bitmask25 = 0x01ffffff;
global const U32 bitmask26 = 0x03ffffff;
global const U32 bitmask27 = 0x07ffffff;
global const U32 bitmask28 = 0x0fffffff;
global const U32 bitmask29 = 0x1fffffff;
global const U32 bitmask30 = 0x3fffffff;
global const U32 bitmask31 = 0x7fffffff;
global const U32 bitmask32 = 0xffffffff;

global const U32 bit1  = (1<<0);
global const U32 bit2  = (1<<1);
global const U32 bit3  = (1<<2);
global const U32 bit4  = (1<<3);
global const U32 bit5  = (1<<4);
global const U32 bit6  = (1<<5);
global const U32 bit7  = (1<<6);
global const U32 bit8  = (1<<7);
global const U32 bit9  = (1<<8);
global const U32 bit10 = (1<<9);
global const U32 bit11 = (1<<10);
global const U32 bit12 = (1<<11);
global const U32 bit13 = (1<<12);
global const U32 bit14 = (1<<13);
global const U32 bit15 = (1<<14);
global const U32 bit16 = (1<<15);
global const U32 bit17 = (1<<16);
global const U32 bit18 = (1<<17);
global const U32 bit19 = (1<<18);
global const U32 bit20 = (1<<19);
global const U32 bit21 = (1<<20);
global const U32 bit22 = (1<<21);
global const U32 bit23 = (1<<22);
global const U32 bit24 = (1<<23);
global const U32 bit25 = (1<<24);
global const U32 bit26 = (1<<25);
global const U32 bit27 = (1<<26);
global const U32 bit28 = (1<<27);
global const U32 bit29 = (1<<28);
global const U32 bit30 = (1<<29);
global const U32 bit31 = (1<<30);
global const U32 bit32 = (1<<31);

#endif