#ifndef RENDER_H
#define RENDER_H

#include "platform.h"
#include "types.h"



typedef struct {
    f32 r, g, b, a;
} Colour;


bool init_renderer(const Window* window);
void destroy_renderer();


void push_rect( f32 x, f32 y, f32 width, f32 hight, Colour colour);


void frame_begin(Window* window);
void frame_end();

#endif

