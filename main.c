#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include "platform.h"
#include "render.h"


int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE h_instance, PWSTR cmd, int show)
{
    
    (void) h_instance;
    (void) cmd;
    
    Window window = {0};
    
    create_window_win32(&window, instance, show);
    
    if(!start_window(&window, L"hand made", 800, 400)) 
    {
        return 1;
    }
    
    init_renderer(&window);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        frame_begin(&window);
        
        push_rect(100, 50, 200, 200, (Colour){1.0f,0.0f,0.0f,1.0f});
        
        push_rect(200, 200, 10, 10, (Colour){0.0f,0.0f,1.0f,1.0f});
        
        frame_end();
    }
    
    return 0;
}

