#ifndef RENDER_H_
#define RENDER_H_

void InitRender(HWND hwnd);
void RenderBegin(HWND hwnd);

void DrawRectangle( u32 x, u32 y, u32 width, u32 hight, u32 colour);

void RenderEnd();

#endif

