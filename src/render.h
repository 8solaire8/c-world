#ifndef RENDER_H
#define RENDER_H

#include "platform.h"
#include "types.h"



typedef struct {
    F32 r, g, b, a;
} Colour;


bool init_renderer(const Window* window);
void destroy_renderer();


void push_rect( F32 x, F32 y, F32 width, F32 hight, Colour colour);


void frame_begin(Window* window);
void frame_end();

#endif

