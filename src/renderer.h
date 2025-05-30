#ifndef _GAME_RENDERER_H
#define _GAME_RENDERER_H

#include <zap.h>

#define COBJMACROS
#define CINTERFACE
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#define RENDERER_D3D_VS_ENTRYPOINT "VSMain"
#define RENDERER_D3D_PS_ENTRYPOINT "PSMain"

#if _DEBUG
  #define RENDERER_D3D_SHADER_COMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG
#else
  #define RENDERER_D3D_SHADER_COMPILE_FLAGS D3DCOMPILE_ENABLE_STRICTNESS
#endif

typedef struct Renderer {
  int width;
  int height;

  zap_window_t window;
  zap_display_t display;

  HWND hwnd;
  ID3D11Device* device;
  IDXGISwapChain* swap_chain;
  ID3D11DeviceContext* device_ctx;
  ID3D11RenderTargetView* rtv;

  bool inited;
} Renderer;

bool renderer_init(Renderer* renderer, zap_window_t window);
bool renderer_compile_pixel_shader(Renderer* renderer, const char* src, size_t src_len, ID3D11PixelShader* out_shader);
bool renderer_compile_vertex_shader(Renderer* renderer, const char* src, size_t src_len, ID3D11VertexShader* out_shader);
void renderer_clear_color(Renderer* renderer, float color[4]);
void renderer_present(Renderer* renderer);
void renderer_destroy(Renderer* renderer);

#endif // _GAME_RENDERER_H
