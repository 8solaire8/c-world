#define COBJMACROS
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>


#include "render.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

static const char shader[] =
    "float4 vs(float2 p : POS) : SV_Position { return float4(p, 0, 1); }\n"
    "float4 ps() : SV_Target { return float4(1, 0.5, 0, 1); }\n";

static ID3D11DeviceContext *ctx;
static ID3D11RenderTargetView *rtv;

static ID3D11InputLayout *il;
static ID3D11Device *dev;
static IDXGISwapChain *sc;


static ID3D11Buffer *vb;

static ID3D11VertexShader *vs;
static ID3D11PixelShader *ps;


void InitRender(HWND hwnd) {
    // 2. Device + swap chain
    DXGI_SWAP_CHAIN_DESC sd = {0};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
            NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &sc, &dev, NULL, &ctx)))
    {
        exit(EXIT_FAILURE);
    }

    // 3. Render target view from the back buffer
    ID3D11Texture2D *bb;
    IDXGISwapChain_GetBuffer(sc, 0, &IID_ID3D11Texture2D, (void **)&bb);
    ID3D11Device_CreateRenderTargetView(dev, (ID3D11Resource *)bb, NULL, &rtv);
    ID3D11Texture2D_Release(bb);

    // 4. Shaders
    ID3DBlob *vsb, *psb;
    D3DCompile(shader, sizeof shader, NULL, NULL, NULL, "vs", "vs_4_0", 0, 0, &vsb, NULL);
    D3DCompile(shader, sizeof shader, NULL, NULL, NULL, "ps", "ps_4_0", 0, 0, &psb, NULL);
    
    ID3D11Device_CreateVertexShader(dev, ID3D10Blob_GetBufferPointer(vsb),
        ID3D10Blob_GetBufferSize(vsb), NULL, &vs);
    ID3D11Device_CreatePixelShader(dev, ID3D10Blob_GetBufferPointer(psb),
        ID3D10Blob_GetBufferSize(psb), NULL, &ps);

    // 5. Input layout + vertex buffer
    D3D11_INPUT_ELEMENT_DESC ied = { "POS", 0, DXGI_FORMAT_R32G32_FLOAT,
        0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
    

    ID3D11Device_CreateInputLayout(dev, &ied, 1, ID3D10Blob_GetBufferPointer(vsb),
        ID3D10Blob_GetBufferSize(vsb), &il);

    float verts[] = { 0.0f, 0.5f,   0.5f, -0.5f,   -0.5f, -0.5f }; // clockwise
    D3D11_BUFFER_DESC bd = { sizeof verts, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER };
    D3D11_SUBRESOURCE_DATA srd = { verts };
    ID3D11Device_CreateBuffer(dev, &bd, &srd, &vb);
}

void 
RenderBegin(HWND hwnd) 
{
    RECT r;
    GetClientRect(hwnd, &r);
    D3D11_VIEWPORT vp = { 0, 0, (float)(r.right - r.left),
                            (float)(r.bottom - r.top), 0, 1 };
    float clear[4] = { 0.1f, 0.2f, 0.4f, 1.0f };
    UINT stride = 2 * sizeof(float), offset = 0;

    ID3D11DeviceContext_OMSetRenderTargets(ctx, 1, &rtv, NULL);
    ID3D11DeviceContext_ClearRenderTargetView(ctx, rtv, clear);
    ID3D11DeviceContext_RSSetViewports(ctx, 1, &vp);
    ID3D11DeviceContext_IASetPrimitiveTopology(ctx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_IASetInputLayout(ctx, il);
}

void 
DrawRectangle( u32 x, u32 y, u32 width, u32 hight, u32 colour) 
{
    ID3D11DeviceContext_IASetVertexBuffers(ctx, 0, 1, &vb, &stride, &offset);
    ID3D11DeviceContext_VSSetShader(ctx, vs, NULL, 0);
    ID3D11DeviceContext_PSSetShader(ctx, ps, NULL, 0);
    
    ID3D11DeviceContext_Draw(ctx, 3, 0);
}

void GetTextWidth(char* text, u32 size ) {

}

void DrawText( u32 x, u32 y, char* text ) {

}

void RenderEnd() {
    IDXGISwapChain_Present(sc, 1, 0);  // 1 = vsync
}
        
