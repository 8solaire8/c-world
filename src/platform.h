/* date = October 18th 2026 11:25 am */

#ifndef PLATFORM_H
#define PLATFORM_H
#include <windows.h>
#include "types.h"

// TODO: make this opac to work on any
//typedef struct Window Window;

typedef struct
{
    HWND hwnd;
    HINSTANCE instance;
    int show;
} Window;

void create_window_win32(Window* window, HINSTANCE instance, int show);

void get_window_size(const Window* window, u16* width, u16* height);

bool start_window(Window* window, const wchar_t* name, u16 width, u16 hight);


#endif //PLATFORM_H
