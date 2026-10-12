#ifndef RENDER_H_
#define RENDER_H_

#include "types.h"

typedef struct {
    u8 r, g, b, a;
} Colour;




void InitRenderer(HWND hwnd);
void DestroyRenderer();


void PushRect( u32 x, u32 y, u32 width, u32 hight, Colour colour);


void FrameBegin(HWND hwnd);
void FrameEnd();

#endif

