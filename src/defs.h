#ifndef DEF_H
#define DEF_H

#include "types.h"

#define PAGE_SIZE 4096

#define KILOBYTES(x) ((U64)(x)     * 1024ULL)
#define MEGABYTES(x) (KILOBYTES(x) * 1024ULL)
#define GIGABYTES(x) (MEGABYTES(x) * 1024ULL)
#define PAGES(x) (PAGE_SIZE * x)
#endif