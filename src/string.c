
#include "string.h"

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


