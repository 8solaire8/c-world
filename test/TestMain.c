#include "arena.h"
#include "types.h"

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
