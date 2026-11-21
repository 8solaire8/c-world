#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

typedef struct W32_Window W32_Window;
struct W32_Window
{
    HWND hwnd;
    HINSTANCE instance;
    S32 show;
};

void w32_create_window(Window* window, HINSTANCE instance, int show);

void w32_get_window_size(const Window* window, F32* width, F32* height);

bool w32_start_window(W32_Window* w32_window, String8 name, U64 width, U64 hight);



#endif // WIN32_WINDOW_H
