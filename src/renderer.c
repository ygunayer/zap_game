#include <stdio.h>
#include <assert.h>

#include "renderer.h"

const char* translate_d3d_error(HRESULT result) {
  switch (result) {
    case D3D11_ERROR_FILE_NOT_FOUND:
      return "The file was not found.";

    case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
      return "There are too many unique instances of a particular type of state object.";

    case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
      return "There are too many unique instances of a particular type of view object.";

    case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
      return "The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.";

    case DXGI_ERROR_INVALID_CALL:
      return "The method call is invalid. For example, a method's parameter may not be a valid pointer.";

    case DXGI_ERROR_WAS_STILL_DRAWING:
      return "The previous blit operation that is transferring information to or from this surface is incomplete.";

    case E_FAIL:
      return "Attempted to create a device with the debug layer enabled and the layer is not installed.";

    case E_INVALIDARG:
      return "An invalid parameter was passed to the returning function.";

    case E_OUTOFMEMORY:
      return "Direct3D could not allocate sufficient memory to complete the call.";

    case E_NOTIMPL:
      return "The method call isn't implemented with the passed parameter combination.";

    default:
      return "Unknown error code";
  }
}

bool log_failure(HRESULT result, const char* msg) {
#if _DEBUG
  printf("%s: %s", msg, translate_d3d_error(result));
#else
  UNUSED(result);
  UNUSED(msg);
#endif
  return false;
}

bool log_shader_compile_error(HRESULT result, ID3DBlob* blob_err) {
  if (!blob_err) {
    return log_failure(result, "Failed to compile shader");
  }

  char compile_err[1024];
  strncpy(compile_err, blob_err->lpVtbl->GetBufferPointer(blob_err), blob_err->lpVtbl->GetBufferSize(blob_err));
  printf("Failed to compile pixel shader due to: %s", compile_err);
  blob_err->lpVtbl->Release(blob_err);
  return false;
}

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
    return log_failure(init_result, "Failed to initialize the device or swap chain");
  }

  ID3D11Texture2D* render_buf = NULL;
  init_result = IDXGISwapChain_GetBuffer(renderer->swap_chain, 0, &IID_ID3D11Texture2D, (void**)&render_buf);
  if (FAILED(init_result) || !render_buf) {
    // TODO propagate error
    return log_failure(init_result, "Failed to create render target view");
  }

  init_result = ID3D11Device_CreateRenderTargetView(renderer->device, (ID3D11Resource*)render_buf, NULL, &renderer->rtv);
  if (FAILED(init_result) || !renderer->rtv) {
    // TODO propagate error
    return log_failure(init_result, "Failed to create render target view");
  }

  ID3D11Texture2D_Release(render_buf);

  ID3D11DeviceContext_OMSetRenderTargets(renderer->device_ctx, 1, &renderer->rtv, NULL);
  D3D11_VIEWPORT viewport = {
    .Width = renderer->width,
    .Height = renderer->height,
  };
  ID3D11DeviceContext_RSSetViewports(renderer->device_ctx, 1, &viewport);

  return true;
}

void renderer_clear_color(Renderer* renderer, float color[4]) {
  ID3D11DeviceContext_ClearRenderTargetView(renderer->device_ctx, renderer->rtv, color);
}

void renderer_present(Renderer* renderer) {
  IDXGISwapChain_Present(renderer->swap_chain, 0, 0);
}

bool renderer_compile_pixel_shader(Renderer* renderer, const char* src, size_t src_len, ID3D11PixelShader* out_shader) {
  assert(renderer);

  ID3DBlob* blob_ps = NULL;
  ID3DBlob* blob_err = NULL;

  HRESULT result = D3DCompile(
    src,
    src_len,
    NULL,
    NULL,
    NULL,
    RENDERER_D3D_PS_ENTRYPOINT,
    "ps_5_0",
    RENDERER_D3D_SHADER_COMPILE_FLAGS,
    0,
    &blob_ps,
    &blob_err
  );

  if (FAILED(result) || !blob_ps) {
    return log_shader_compile_error(result, blob_err);
  }

  result = ID3D11Device_CreatePixelShader(
    renderer->device,
    blob_ps->lpVtbl->GetBufferPointer(blob_ps),
    blob_ps->lpVtbl->GetBufferSize(blob_ps),
    NULL,
    &out_shader
  );
  if (FAILED(result)) {
    return log_failure(result, "Failed to create pixel shader");
  }

  return true;
}

bool renderer_compile_vertex_shader(Renderer* renderer, const char* src, size_t src_len, ID3D11VertexShader* out_shader) {
  assert(renderer);

  ID3DBlob* blob_ps = NULL;
  ID3DBlob* blob_err = NULL;

  HRESULT result = D3DCompile(
    src,
    src_len,
    NULL,
    NULL,
    NULL,
    RENDERER_D3D_VS_ENTRYPOINT,
    "vs_5_0",
    RENDERER_D3D_SHADER_COMPILE_FLAGS,
    0,
    &blob_ps,
    &blob_err
  );

  if (FAILED(result) || !blob_ps) {
    return log_shader_compile_error(result, blob_err);
  }

  result = ID3D11Device_CreateVertexShader(
    renderer->device,
    blob_ps->lpVtbl->GetBufferPointer(blob_ps),
    blob_ps->lpVtbl->GetBufferSize(blob_ps),
    NULL,
    &out_shader
  );
  if (FAILED(result)) {
    return log_failure(result, "Failed to create vertex shader");
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

  if (renderer->rtv) {
    ID3D11RenderTargetView_Release(renderer->rtv);
    renderer->rtv = NULL;
  }

  if (renderer->device) {
    ID3D11Device_Release(renderer->device);
    renderer->device = NULL;
  }

  if (renderer->device_ctx) {
    ID3D11DeviceContext_Release(renderer->device_ctx);
    renderer->device_ctx = NULL;
  }

  renderer->inited = false;
}
