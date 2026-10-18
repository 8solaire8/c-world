#include "platform.h"




void
get_window_size(const Window* window, u16* width, u16* hight )
{
    RECT rect;
    if ( GetWindowRect(window->hwnd, &rect) )
    {
        *width = rect.right - rect.left;
        *hight = rect.bottom - rect.top;
    }
}


LRESULT CALLBACK 
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void
create_window_win32(Window* window, HINSTANCE instance, int show)
{
    window->instance = instance;
    window->show = show;
}


bool
start_window(Window* window, const wchar_t* CLASS_NAME, u16 width, u16 hight)
{
    WNDCLASS wc = { };
    
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = window->instance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    HWND hwnd = CreateWindowEx
    (
     0,                              // Optional window styles.
     CLASS_NAME,                     // Window class
     CLASS_NAME,
     WS_OVERLAPPEDWINDOW,            // Window style
     
     // Size and position
     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
     NULL,       // Parent window   
     NULL,       // Menu
     window->instance,  // Instance handle
     NULL        // Additional application data
     );
    
    if (hwnd == NULL)
    {
        return false;
    }
    
    window->hwnd = hwnd;
    
    ShowWindow(window->hwnd, window->show);
    return true;
}

