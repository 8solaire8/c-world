#ifndef WIN32_RENDER_H
#define WIN32_RENDER_H

typedef struct Colour Colour;
struct Colour 
{
    F32 r, g, b, a;
};


bool renderer_create(const Window* window);
void renderer_release();


void push_rect( F32 x, F32 y, F32 width, F32 hight, Colour colour);

void frame_begin(Window* window);
void frame_end();

#endif //WIN32_RENDER_H

