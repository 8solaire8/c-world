#ifndef DEF_H
#define DEF_H

#include "types.h"

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

#define CPU_NUMBER 32

#endif