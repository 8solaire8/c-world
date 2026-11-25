#include "base/base_inc.h"
#include "window/window_inc.h"
#include "audio/audio_inc.h"

#ifdef OS_WINDOWS
#include "win32/win32_inc.h"
#elif OS_LINUX
#include "linux/linux_inc.h"
#endif

#include "base/base_inc.c"
#include "window/window_inc.c"
#include "audio/audio_inc.c"


#ifdef OS_WINDOWS
#include "win32/win32_inc.c"
#elif OS_LINUX
#include "linux/linux_inc.c"
#endif


static Arena *g_arena;

#ifdef OS_WINDOWS
int WINAPI 
wWinMain(HINSTANCE instance, HINSTANCE h_instance, PWSTR cmd, S32 show) 
#elif OS_LINUX
int main(int argv, char* argc) 
#endif
{
    os_create_subsystem();
    
    g_arena = arena_create();
    context_create(0, 1);
    
    String8 name = str8_cstring("The Adventures of Drosten");
    
    Window window = window_create(name, 1920, 1080);
    
    // sound functions
    load_sounds(g_arena);
    if(!audio_create()) return 1;
    audio_thread_start();
    
    renderer_create(window);
    
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        frame_begin(window);
        
        push_rect(100, 50, 30, 30, (Colour){1.0f,0.0f,0.0f,1.0f});
        
        push_rect(300, 300, 300, 250, (Colour){0.0f,1.0f,1.0f,1.0f});
        
        push_rect(500, 500, 30, 30, (Colour){0.0f,0.0f,1.0f,1.0f});
        
        frame_end();
    }
    
    return 0;
}

