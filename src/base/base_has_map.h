#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "types.h"

#define freeHashTable(ptr) do {   \
_freeHashTable(ptr);          \
(ptr) = NULL;                 \
} while(0);                  

typedef struct {
    char* key;
    i64 value;
    bool wt;
} HashItem;

typedef struct {
    u64 size;
    u64 capacity;
    HashItem* items;
} HashTable;


HashTable* initHashTable();
void _freeHashTable(HashTable* table);

i64 getHashValue(const HashTable* table, char* key);
void setHashValue(HashTable* table, char* key, i64 value);

void printHashTable(HashTable* table);


#endif //HASH_TABLE_H_
