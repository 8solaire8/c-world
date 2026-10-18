#ifndef RENDER_H
#define RENDER_H

#include "platform.h"
#include "types.h"


typedef struct {
    u8 r, g, b, a;
} Colour;


bool init_renderer(const Window* window);
void destroy_renderer();


void push_rect( u32 x, u32 y, u32 width, u32 hight, Colour colour);


void frame_begin(const Window* window);
void frame_end();

#endif

