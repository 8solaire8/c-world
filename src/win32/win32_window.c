internal W32_Window*
w32_window_from_handle(Window handle)
{
    W32_Window* window = (W32_Window*)handle.u64[0];
    return window;
}

internal void
get_window_size(Window handle, F32* width, F32* hight )
{
    W32_Window* window = w32_window_from_handle(handle);
    RECT rect;
    if ( GetWindowRect(window->hwnd, &rect) )
    {
        *width = (F32)(rect.right - rect.left);
        *hight = (F32)(rect.bottom - rect.top);
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

Window
window_create(String8 name, U64 width, U64 hight)
{
    Window handle = {0};
    handle.u64[0] = IntFromPtr((push_struct_zero(w32_state.arena, W32_Window)));
    
    W32_Window* window = w32_window_from_handle(handle);
    window->instance = GetModuleHandle(NULL);
    
    WNDCLASS wc = { 0 };
    
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = window->instance;
    wc.lpszClassName = L"FOR LATER";
    
    RegisterClass(&wc);
    
    window->hwnd = CreateWindowEx
    (
     0,                              // Optional window styles.
     wc.lpszClassName,                     // Window class
     wc.lpszClassName,
     WS_OVERLAPPEDWINDOW,            // Window style
     
     // Size and position
     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
     NULL,       // Parent window   
     NULL,       // Menu
     window->instance,  // Instance handle
     NULL        // Additional application data
     );
    
    ShowWindow(window->hwnd, 1);
    return handle;
}

