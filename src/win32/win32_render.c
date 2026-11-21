#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "platform.h"
#include "defs.h"
#include "render.h"
#include "arena.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#define VERTEX_BUFFER_SIZE 4096
#define INDEX_BUFFER_SIZE 512

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
    F32 x,y;
    Colour colour;
} Vertex;


static U64     g_vertex_buffer_count = 0;
static Vertex *g_vertex_buffer;

static U32  g_index_buffer_count = 0;
static U16 *g_index_buffer;

bool
renderer_create(const Window* window) 
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
    F32 width, height;
    get_window_size(window, &width, &height);
    
    D3D11_VIEWPORT vp = { 0, 0, width, height, 0, 1 };
    
    F32 clear[4] = { 0.1f, 0.6f, 0.4f, 1.0f };
    
    g_index_buffer_count = 0;
    g_vertex_buffer_count = 0;
    
    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rtv, NULL);
    ID3D11DeviceContext_ClearRenderTargetView(ctx, rtv, clear);
    ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_IASetInputLayout(ctx, il);
    
    
    D3D11_MAPPED_SUBRESOURCE mapped;
    
    ID3D11DeviceContext_Map(ctx, (ID3D11Resource*)vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    
    g_vertex_buffer = (Vertex*)mapped.pData;
    
    ID3D11DeviceContext_Map(ctx, (ID3D11Resource*)ib, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    
    g_index_buffer = (U16*)mapped.pData;
}

void 
frame_end() 
{
    D3D11_MAPPED_SUBRESOURCE mapped = {0};
    
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource*)vb, 0);
    ID3D11DeviceContext_Unmap(ctx, (ID3D11Resource*)ib, 0);
    
    g_vertex_buffer = NULL;
    g_index_buffer  = NULL;
    
    
    U32 stride = sizeof(Vertex);
    U32 offset = 0;
    ID3D11DeviceContext_IASetVertexBuffers(ctx, 0, 1, &vb, &stride, &offset);
    ID3D11DeviceContext_IASetIndexBuffer(ctx, ib, DXGI_FORMAT_R16_UINT, 0);
    ID3D11DeviceContext_VSSetShader(ctx, vs, NULL, 0);
    ID3D11DeviceContext_PSSetShader(ctx, ps, NULL, 0);
    
    ID3D11DeviceContext_DrawIndexed(ctx, g_index_buffer_count, 0, 0); 
    IDXGISwapChain_Present(sc, 1, 0);
}


void 
PushTryangle(U32 x, U32 y, U32 size) 
{ 
    
}



U32 
screen_to_shader(U16 point)
{
    return 0;
}

void 
push_rect( F32 x, F32 y, F32 width, F32 height, Colour colour) 
{
    F32 screen_width, screen_height;
    get_window_size(g_window, &screen_width, &screen_height);
    
    F32 left   = (x / screen_width) * 2.0f - 1.0f;
    F32 right  = ((x + width) / screen_width) * 2.0f - 1.0f;
    F32 top    = 1.0f - (y / screen_height) * 2.0f;
    F32 bottom = 1.0f - ((y + height) / screen_height) * 2.0f;
    
    U16 base = (U16)g_vertex_buffer_count;
    
    g_vertex_buffer[g_vertex_buffer_count++] = (Vertex) { left ,  top,    colour };
    g_vertex_buffer[g_vertex_buffer_count++] = (Vertex) { right,  top,    colour };
    g_vertex_buffer[g_vertex_buffer_count++] = (Vertex) { left,   bottom, colour };
    g_vertex_buffer[g_vertex_buffer_count++] = (Vertex) { right,  bottom, colour };
    
    g_index_buffer[g_index_buffer_count++] = base + 0;
    g_index_buffer[g_index_buffer_count++] = base + 1;
    g_index_buffer[g_index_buffer_count++] = base + 2;
    g_index_buffer[g_index_buffer_count++] = base + 2;
    g_index_buffer[g_index_buffer_count++] = base + 1;
    g_index_buffer[g_index_buffer_count++] = base + 3;
}



