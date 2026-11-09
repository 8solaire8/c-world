#include <windows.h>

#include "platform.h"
#include "render.h"
#include "audio.h"
#include "arena.h"
#include "defs.h"
#include "lane.h"

static Arena g_arena;

#ifdef _WIN32
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE h_instance, PWSTR cmd, S32 show) 

#else
int main(int argv, char* argc) 
#endif
{
    UNUSED(h_instance);
    UNUSED(cmd);
    
    arena_create(&g_arena, GIGABYTES(4));
    context_create(0, 1);
    
    Window window = {0};
    
    create_window_win32(&window, instance, show);
    
    if(!start_window(&window, L"The Adventures of Drosten", 800, 400)) 
    {
        return 1;
    }
    // sound functions
    load_sounds(&g_arena);
    if(!audio_create()) return 1;
    audio_thread_start();
    
    renderer_create(&window);
    
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

