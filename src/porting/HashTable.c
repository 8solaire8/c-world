#include "HashTable.h"

//TO DO fix the low end of the resizing causing crashes 28 is min for some reson

#define DEFAULT_START_SIZE 10
#define RESIZE_FACTOR 2

static void reorderHashMap( HashTable* table);

int compare_strings(const char str1[static 1], const char str2[static 1]) {
    if (str1 == NULL && str2 == NULL) {
        return 0; 
    }
    if (str1 == NULL) {
        return -1;
    }
    if (str2 == NULL) {
        return 1; 
    }
    return strcmp(str1, str2); 
}

//TO DO fix this warning
static u32 hashFunc(Byte* data) {
    assert(data && "data cannot be null");
    u32 res = 5381;
    int c;
    while (c = *data++) {
        res = ((res << 5) + res) + c;
    }
    return res;
}

static i64 getHashSpot(const HashTable* table, char* key ){
    assert(key && "key is not valid");
    u32 spot = hashFunc(key) % table->capacity;
    assert(table->items && "items is not allocated in hash table");
    
    for (size_t i = 0; i < table->capacity  && table->items[spot].wt &&
                       (compare_strings(table->items[spot].key, key) != 0);
         ++i) {
        spot = (spot + 1) % table->capacity;
    }
    return spot;
}

static void resizeHashTable(HashTable table[static 1], const u64 new_size){
    assert(new_size > table->capacity && "new size is smaller than old size");
    table->capacity = new_size;
    table->items = realloc(table->items, table->capacity * sizeof(*table->items));
    for(int i = 0; i < table->capacity; ++i) {
	if( table->items[i].key == NULL ) {   
	    table->items[i].key = calloc(1, sizeof(char));
	}
    }
}


HashTable* initHashTable() {
    HashTable* table = calloc(1, sizeof(HashTable));
    assert(table);
    table->size = 0;
    table->capacity = DEFAULT_START_SIZE;
    table->items = calloc(table->capacity, sizeof(*table->items) );
    for(int i = 0; i < table->capacity; ++i){
	table->items[i].key = calloc(1, sizeof(char));
    }
    assert(table->items && "failed to allocate space for table items");
    return table;
}

void _freeHashTable(HashTable* table){
    if (table != NULL) {
        if (table->items != NULL) {
	    for(int spot=0; spot < table->capacity; ++spot) {
		if(table->items[spot].wt || table->items[spot].key != NULL) {
		    free(table->items[spot].key);
		    table->items[spot].key = NULL;
		    table->items[spot].wt = false;
		}
	    }
            free(table->items);
            table->items = NULL;
        }
        free(table);   
    } 
}

i64 getHashValue(const HashTable* table, char* key) {
    i64 spot = getHashSpot(table, key);
    if (compare_strings(table->items[spot].key, key) == 0) {
	return table->items[spot].value;
    }
    return -1;
}

void setHashValue(HashTable* table, char* key, i64 value){
    assert(table && "table does not exisit");
    assert(table->items && "items does not exist in table");
    assert(key && "no key given");
    i64 spot = getHashSpot(table, key);
    assert(spot >=0 && "value does not exist");
    if (table->items[spot].wt) {
        if (compare_strings(table->items[spot].key, key) != 0) {
	    reorderHashMap(table);
        }
	else {
	    table->items[spot].value = value;
	}
    } else {
	size_t string_size = strlen(key) + 1;
	
        table->items[spot].key = realloc(table->items[spot].key, string_size);
	memset(table->items[spot].key, 0, string_size );
	memcpy(table->items[spot].key, key, string_size);	
        table->items[spot].wt = true;
        table->items[spot].value = value;
        table->size += 1;
    }
}

static void copyHashItem(HashItem dest[static 1], HashItem src[static 1]){
    dest->wt  = src->wt;
    dest->value = src->value;
    
    size_t key_size = strlen(src->key) + 1;
    if(dest->key == NULL) {
	dest->key = calloc(key_size, sizeof(char));
    }
    else{
	dest->key = realloc(dest->key, key_size);
    }
    memcpy(dest->key, src->key, key_size);
}

static void reorderHashMap( HashTable *table ){
    assert(table && "table does not exist for reordering");
    HashTable* new_table = initHashTable();
    resizeHashTable(new_table, table->capacity * 2);
    
    assert(new_table->items != NULL && "realloc failed");
    for(int i = 0; i < table->capacity; ++i) {
	if( table->items[i].wt && table->items[i].key ) {
	    setHashValue(new_table, table->items[i].key, table->items[i].value); 
	    table->items[i].wt = false;
	    table->items[i].value = 0;
	    memset(table->items[i].key, 0, strlen(table->items[i].key));
	}
    }
    resizeHashTable(table, table->capacity * 2);
    
    for(int i = 0; i < new_table->capacity; ++i) {
	if( new_table->items[i].wt && new_table->items[i].key ) {
	    copyHashItem(&table->items[i], &new_table->items[i]);
	}
	
    }
    freeHashTable(new_table);
}

void printHashTable(HashTable* table){
    assert(table);
    for( int i = 0; i < table->capacity; ++i) {
	if ( table->items[i].wt ) {
	    printf("value = %ld; key = %s\n", table->items[i].value, table->items[i].key );
	}
    }

}
