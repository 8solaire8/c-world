#include <stdio.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

void DrawRect() {

}

void DrawText() {

}

unsigned long GetGreenPixel(Display* display, int screen) {
    Colormap cmap = DefaultColormap(display, screen);
    XColor exact_color, screen_color;

    // Allocate the color from the system colormap
    XAllocNamedColor(display, cmap, "green", &exact_color, &screen_color);

    // Return the hardware-specific pixel value
    return screen_color.pixel;
}


int main(int argc, char* argv[]){

    Display* display = XOpenDisplay(NULL);

    if (display == NULL ) {
        fprintf(stderr, "Failed to initialise display");
        return EXIT_FAILURE;
    }

    Window root = DefaultRootWindow(display);

    if(root == None) {
        fprintf(stderr, "No root window found");
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, root, 0,0, 800, 600, 0, BlackPixel(display, screen), BlackPixel(display, screen));

    if(window == None) {
        fprintf(stderr, "No Failed to create winow");
        XCloseDisplay(display);
        return EXIT_FAILURE;
    }


    GC gc = XCreateGC(display, window, 0, NULL);

    if (gc == None) {
        fprintf(stderr, "failed to create gc");
        return EXIT_FAILURE;
    }

    XSetForeground(display, gc, WhitePixel(display, screen));
    //XSetBackground(display, gc, BlackPixel(display, screen));

    int line_width = 2;
    int line_style = LineSolid;
    int cap_style = CapButt;
    int join_style = JoinBevel;

    XSetLineAttributes(display, gc, line_width, line_style, cap_style, join_style);

  /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(display, gc, FillSolid);


    XSelectInput(display, window, ExposureMask | PointerMotionMask | KeyPressMask);

    XMapWindow(display, window);
    XFlush(display);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, & wm_delete_window, 1);

    Pixmap backbuffer = XCreatePixmap(
        display,
        window,
        800,
        600,
        DefaultDepth(display, screen)
    );


    typedef struct Point {
        int x;
        int y;
    } Point;

    XEvent event;
    bool quited = false;
    while (!quited) {
        XNextEvent(display, &event);
        Point p = {0};

        switch ( event.type ) {
            case ClientMessage:
                if ( event.xclient.data.l[0] == wm_delete_window ) {
                    XDestroyWindow(event.xclient.display, event.xclient.window);
                    quited = true;
                }
                break;
            case Expose:
                if (event.xexpose.count == 0) {
                    printf("updating\n");
                }
            break;
            case MotionNotify:
                    p.x = event.xmotion.x;
                    p.y = event.xmotion.y;
                    //printf("x: %d, y: %d\n", event.xmotion.x, event.xmotion.y);
                break;
            case KeyPress:
                printf("%d\n", event.xkey.keycode);
                break;
            default:
                break;
        }

        //XClearWindow(display, window);
        XSetForeground(display, gc, BlackPixel(display, screen));
        XFillRectangle(display, backbuffer, gc, 0, 0, 800, 600);

        XSetForeground(display, gc, WhitePixel(display, screen));
        XFillRectangle(display, backbuffer, gc, p.x - 50, p.y -40, 100, 80);

        XSetForeground(display, gc, GetGreenPixel(display, screen));
        XFillRectangle(display, backbuffer, gc, 50, 50, 200, 200);

        XCopyArea(display, backbuffer, window, gc, 0, 0, 800, 600, 0, 0 );

        XFlush(display);
        //usleep(1600);
    }

    return EXIT_SUCCESS;
}
