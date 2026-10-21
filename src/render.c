#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "defs.h"
#include "render.h"
#include "arena.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#define VERTEX_BUFFER_SIZE 4096
#define INDEX_BUFFER_SIZE 512

//static const char shader[] =
//"float4 vs(float2 p : POS) : SV_Position { return float4(p, 0, 1); }\n"
//"float4 ps() : SV_Target { return float4(1, 0.5, 0, 1); }\n";

static const char shader[] =
"struct VSIn  { float2 pos : POSITION; float4 col : COLOR; };\n"
"struct VSOut { float4 pos : SV_POSITION; float4 col : COLOR; };\n"
"VSOut vs_main(VSIn i) { VSOut o; o.pos = float4(i.pos, 0, 1); o.col = i.col; return o; }\n"
"float4 ps_main(VSOut i) : SV_TARGET { return i.col; }\n";

static ID3D11DeviceContext *ctx;
static ID3D11RenderTargetView *rtv;

static ID3D11Texture2D *bb;

static ID3D11InputLayout *il;
static ID3D11Device *dev;
static IDXGISwapChain *sc;

static ID3D11VertexShader *vs;
static ID3D11PixelShader *ps;


static ID3D11Buffer *vb;
static ID3D11Buffer *ib;

typedef struct 
{
    f32 x,y;
    Colour colour;
} Vertex;




bool
init_renderer(const Window* window) 
{
    DXGI_SWAP_CHAIN_DESC sd = {0};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window->hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
                                             NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &sc, &dev, NULL, &ctx)))
    {
        return false;
    }
    
    IDXGISwapChain_GetBuffer(sc, 0, &IID_ID3D11Texture2D, (void **)&bb);
    ID3D11Device_CreateRenderTargetView(dev, (ID3D11Resource *)bb, NULL, &rtv);
    ID3D11Texture2D_Release(bb);
    
    ID3DBlob *vsb, *psb;
    D3DCompile(shader, sizeof shader, NULL, NULL, NULL, "vs_main", "vs_4_0", 0, 0, &vsb, NULL);
    D3DCompile(shader, sizeof shader, NULL, NULL, NULL, "ps_main", "ps_4_0", 0, 0, &psb, NULL);
    
    ID3D11Device_CreateVertexShader(dev, ID3D10Blob_GetBufferPointer(vsb),
                                    ID3D10Blob_GetBufferSize(vsb), NULL, &vs);
    ID3D11Device_CreatePixelShader(dev, ID3D10Blob_GetBufferPointer(psb),
                                   ID3D10Blob_GetBufferSize(psb), NULL, &ps);
    
    
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    
    HRESULT hr = ID3D11Device_CreateInputLayout(dev, ied, ARRAYSIZE(ied),
                                                ID3D10Blob_GetBufferPointer(vsb),
                                                ID3D10Blob_GetBufferSize(vsb), &il);
    if (FAILED(hr))
    {
        return false;
    }
    
    D3D11_BUFFER_DESC bd = { VERTEX_BUFFER_SIZE, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE };
    
    ID3D11Device_CreateBuffer(dev, &bd, NULL, &vb);
    
    D3D11_BUFFER_DESC ibd = { INDEX_BUFFER_SIZE, D3D11_USAGE_DYNAMIC, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE };
    
    ID3D11Device_CreateBuffer(dev, &ibd, NULL, &ib);
    return true;
}

static Window* g_window;

void 
frame_begin(Window* window) 
{
    g_window = window;
    f32 width, height;
    get_window_size(window, &width, &height);
    
    D3D11_VIEWPORT vp = { 0, 0, width, height, 0, 1 };
    
    f32 clear[4] = { 0.1f, 0.6f, 0.4f, 1.0f };
    
    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rtv, NULL);
    ID3D11DeviceContext_ClearRenderTargetView(ctx, rtv, clear);
    ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_IASetInputLayout(ctx, il);
}

void 
frame_end() 
{
    ID3D11DeviceContext_DrawIndexed(ctx, 12, 0, 0); 
    IDXGISwapChain_Present(sc, 1, 0);
}


void 
PushTryangle(u32 x, u32 y, u32 size) 
{ 
    
}



u32 
screen_to_shader(u16 point)
{
    
}

void 
push_rect( f32 x, f32 y, f32 width, f32 height, Colour colour) 
{
    
    f32 screen_width, screen_height;
    get_window_size(g_window, &screen_width, &screen_height);
    
    f32 left   = (x / screen_width) * 2.0f - 1.0f;
    f32 right  = ((x + width) / screen_width) * 2.0f - 1.0f;
    f32 top    = 1.0f - (y / screen_height) * 2.0f;
    f32 bottom = 1.0f - ((y + height) / screen_height) * 2.0f;
    
    Vertex verts[4] = {
        { left,  top,    colour },   // 0
        { right, top,    colour },   // 1
        { left,  bottom, colour },   // 2
        { right, bottom, colour },   // 3
    };
    
    u16 indices[] = { 0, 1, 2,   2, 1, 3 };
    
    D3D11_MAPPED_SUBRESOURCE mapped;
    
    ID3D11DeviceContext_Map(ctx, (ID3D11Resource*)vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, verts, sizeof(verts));
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource*)vb, 0);
    
    ID3D11DeviceContext_Map(ctx, (ID3D11Resource*)ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, indices, sizeof(indices));
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource*)ib, 0);
    
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(ctx, 0, 1, &vb, &stride, &offset);
    ID3D11DeviceContext_IASetIndexBuffer(ctx, ib, DXGI_FORMAT_R16_UINT, 0);
    ID3D11DeviceContext_VSSetShader(ctx, vs, NULL, 0);
    ID3D11DeviceContext_PSSetShader(ctx, ps, NULL, 0);
    
}



