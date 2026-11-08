#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include "platform.h"
#include "render.h"
#include "audio.h"
#include "arena.h"
#include "defs.h"

static Arena g_arena;

#ifdef _WIN32
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE h_instance, PWSTR cmd, int show)
#else
int main(int argv, char* argc)
#endif
{
    
    (void) h_instance;
    (void) cmd;
    
    arena_alloc(&g_arena, GIGABYTES(4));
    init_scratch();
    
    Window window = {0};
    
    create_window_win32(&window, instance, show);
    
    if(!start_window(&window, L"The Adventures of Drosten", 800, 400)) 
    {
        return 1;
    }
    // sound functions
    load_sounds(&g_arena);
    if(!audio_init()) return 1;
    start_audio_thread();
    
    init_renderer(&window);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        frame_begin(&window);
        
        push_rect(100, 50, 30, 30, (Colour){1.0f,0.0f,0.0f,1.0f});
        
        push_rect(300, 300, 300, 250, (Colour){0.0f,1.0f,1.0f,1.0f});
        
        push_rect(500, 500, 30, 30, (Colour){0.0f,0.0f,1.0f,1.0f});
        
        frame_end();
    }
    
    return 0;
}

