#ifndef WINDOW_H
#define WINDOW_H

typedef struct Window Window;

struct Window
{
    U64 u64[1];
};

Window window_create(String8 name, U64 width, U64 height);
void window_release(Window window);
void window_start(Window window);

#endif //WINDOW_MANAGER_H
