#ifndef WIN32_BASE_H
#define WIN32_BASE_H

typedef struct W32_State W32_State;
struct W32_State
{
    Arena *arena;
};


global W32_State w32_state = {0};

//internal void w32_create_subsystem();


#endif //WIN32_BASE_H
