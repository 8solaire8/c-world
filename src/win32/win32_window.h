#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

typedef struct W32_Window W32_Window;
struct W32_Window
{
    HWND hwnd;
    HINSTANCE instance;
};


internal W32_Window* w32_window_from_handle(Window handle);
#endif // WIN32_WINDOW_H
