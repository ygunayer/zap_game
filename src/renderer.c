#include "renderer.h"

#include <assert.h>

bool renderer_init(Renderer *renderer, zap_window_t window) {
  assert(renderer);
  assert(!renderer->inited);
  assert(!renderer->device);
  assert(!renderer->device_ctx);
  assert(!renderer->swap_chain);

  HWND hwnd = zap_window_get_hwnd(window);
  if (!hwnd) {
    // TODO propagate error
    return false;
  }
  renderer->hwnd = hwnd;

  zap_display_t display = zap_window_get_display(window);
  if (!display) {
    // TODO propagate error
    return false;
  }
  renderer->display = display;

  zap_display_info_t display_info = {0};
  if (!zap_display_get_info(display, &display_info)) {
    // TODO propagate error
    return false;
  }

  renderer->width = display_info.rect.width;
  renderer->height = display_info.rect.height;

  DXGI_SWAP_CHAIN_DESC swap_chain_desc = {
    .BufferDesc = {
      .RefreshRate = {
        .Numerator = display_info.refresh_rate,
        .Denominator = 1,
      },
      .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
    },
    .SampleDesc = {
      .Count = 1,
      .Quality = 0,
    },
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = 1,
    .OutputWindow = hwnd,
    .Windowed = display_info.display_mode != ZAP_DISPLAY_MODE_FULLSCREEN,
  };

  D3D_FEATURE_LEVEL feature_level;

  HRESULT init_result = D3D11CreateDeviceAndSwapChain(
    NULL,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
    NULL,
    0,
    D3D11_SDK_VERSION,
    &swap_chain_desc,
    &renderer->swap_chain,
    &renderer->device,
    &feature_level,
    &renderer->device_ctx
  );

  if (FAILED(init_result) || !renderer->swap_chain || !renderer->device || !renderer->device_ctx) {
    // TODO propagate error
    return false;
  }

  return true;
}

void renderer_destroy(Renderer *renderer) {
  if (!renderer) {
    return;
  }

  if (renderer->swap_chain) {
    IDXGISwapChain_Release(renderer->swap_chain);
    renderer->swap_chain = NULL;
  }

  if (renderer->device_ctx) {
    ID3D11DeviceContext_Release(renderer->device_ctx);
    renderer->device_ctx = NULL;
  }

  if (renderer->device) {
    ID3D11Device_Release(renderer->device);
    renderer->device = NULL;
  }

  renderer->inited = false;
}
