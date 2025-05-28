#ifndef _GAME_RENDERER_H
#define _GAME_RENDERER_H

#include <zap.h>

#define COBJMACROS
#define CINTERFACE
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

typedef struct Renderer {
  int width;
  int height;

  zap_window_t window;
  zap_display_t display;

  HWND hwnd;
  ID3D11Device* device;
  IDXGISwapChain* swap_chain;
  ID3D11DeviceContext* device_ctx;

  bool inited;
} Renderer;

bool renderer_init(Renderer* renderer, zap_window_t window);
void renderer_destroy(Renderer* renderer);

#endif // _GAME_RENDERER_H
