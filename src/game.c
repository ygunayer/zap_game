#include <assert.h>
#include <stdio.h>

#include "game.h"

static const char DEFAULT_D3D_VERTEX_SHADER[] =
  "cbuffer g_TransformBuffer: register(b0) {                                       \n"
  "  matrix g_WorldViewProj;                                                       \n"
  "};                                                                              \n"
  "                                                                                \n"
  "struct vs_in {                                                                  \n"
  "  float4 in_Color: COLOR;                                                       \n"
  "  float3 in_Position: POSITION;                                                 \n"
  "  float2 in_TexCoord: TEXCOORD0;                                                \n"
  "};                                                                              \n"
  "                                                                                \n"
  "struct vs_out {                                                                 \n"
  "  float4 out_Position: SV_POSITION;                                             \n"
  "  float4 out_Color: COLOR;                                                      \n"
  "  float2 out_TexCoord: TEXCOORD0;                                               \n"
  "};                                                                              \n"
  "                                                                                \n"
  "vs_out VSMain(vs_in input) {                                                    \n"
  "  vs_out output;                                                                \n"
  "  output.out_Position = mul(float4(input.in_Position, 1.0f), g_WorldViewProj);  \n"
  "  output.out_Color = input.in_Color;                                            \n"
  "  output.out_TexCoord = input.in_TexCoord;                                      \n"
  "  return output;                                                                \n"
  "}                                                                               \n";
static const size_t DEFAULT_D3D_VERTEX_SHADER_LEN = strlen(DEFAULT_D3D_VERTEX_SHADER);

static const char DEFAULT_D3D_PIXEL_SHADER[] =
  "Texture2D g_Texture: register(t0);                                   \n"
  "SamplerState s_Sampler: register(s0);                                \n"
  "                                                                     \n"
  "struct PS_INPUT {                                                    \n"
  "  float4 in_Position: SV_POSITION;                                   \n"
  "  float4 in_Color: COLOR;                                            \n"
  "  float2 in_TexCoord: TEXCOORD0;                                     \n"
  "};                                                                   \n"
  "                                                                     \n"
  "// Pixel shader main function                                        \n"
  "float4 PSMain(PS_INPUT input): SV_TARGET {                           \n"
  "  float4 texColor = g_Texture.Sample(s_Sampler, input.in_TexCoord);  \n"
  "  float4 out_Color = texColor * input.in_Color;                      \n"
  "  return out_Color;                                                  \n"
  "}                                                                    \n";

static const size_t DEFAULT_D3D_PIXEL_SHADER_LEN = strlen(DEFAULT_D3D_PIXEL_SHADER);

bool game_init(Game* game, zap_window_t window) {
  assert(game);

  if (game->inited) {
    // TODO propagate error
    return false;
  }

#if PLATFORM_WINDOWS
  game->hwnd = zap_window_get_hwnd(window);
  if (!game->hwnd) {
    // TODO propagate error
    return false;
  }
#endif

  game_set_target_fps(game, DEFAULT_TARGET_FPS);
  game_set_fixed_fps(game, DEFAULT_FIXED_FPS);

  if (!renderer_init(&game->renderer, window)) {
    // TODO propagate error
    return false;
  }

  ID3D11PixelShader* ps = NULL;
  if (!renderer_compile_pixel_shader(&game->renderer, DEFAULT_D3D_PIXEL_SHADER, DEFAULT_D3D_PIXEL_SHADER_LEN, ps)) {
    return false;
  }

  ID3D11VertexShader* vs = NULL;
  if (!renderer_compile_vertex_shader(&game->renderer, DEFAULT_D3D_VERTEX_SHADER, DEFAULT_D3D_VERTEX_SHADER_LEN, vs)) {
    return false;
  }

  game->inited = true;
  return true;
}

void game_set_target_fps(Game *game, int target_fps) {
  assert(game);

  if (target_fps < 0) {
    // TODO: determine screen FPS
    target_fps = 60;
  }

  game->target_fps = target_fps;
  game->ticks_per_frame = target_fps == 0 ? 0 : (TICKS_PER_SECOND / game->target_fps);
}

void game_set_fixed_fps(Game *game, int fps) {
  assert(game);
  assert(fps);
  game->fixed_fps = fps;
  game->fixed_ticks_per_frame = TICKS_PER_SECOND / game->fixed_fps;
}

void game_tick(Game* game, tick_t ticks) {
  assert(game);
  assert(game->inited);

  if (!game->last_tick) {
    game->last_tick = ticks;
    return;
  }

  tick_t elapsed = ticks - game->last_tick;
  game->last_tick = ticks;
  game->total_ticks += elapsed;

  game->fixed_tick_acc += elapsed;
  while (game->fixed_tick_acc >= game->fixed_ticks_per_frame) {
    game_fixed_update(game, (float)game->fixed_ticks_per_frame / TICKS_PER_SECOND);
    game->fixed_tick_acc -= game->fixed_ticks_per_frame;
    game->num_fixed_updates += 1;
  }

  game->tick_acc += elapsed;
  if (game->target_fps == 0 || game->tick_acc >= game->ticks_per_frame) {
    game_update(game, (float)game->tick_acc / TICKS_PER_SECOND);
    game->tick_acc = 0;
    game->num_updates += 1;
  }

  static tick_t anan;
  static tick_t ananu;
  static tick_t ananfu;
  anan += elapsed;
  if (anan >= TICKS_PER_SECOND) {
    anan = 0;
    tick_t updates = game->num_updates - ananu;
    tick_t fupdates = game->num_fixed_updates - ananfu;
    ananu = game->num_updates;
    ananfu = game->num_fixed_updates;
    char buf[1024] = {0};
    sprintf(buf, "game - Updates: %lld, Fixed Updates: %lld", updates, fupdates);
#if defined(PLATFORM_WINDOWS)
    SetWindowTextA(game->hwnd, TEXT(buf));
#endif
  }
}

void game_update(Game *game, float delta) {
  static float color[4] = {.4f, .3f, .4f, 1.f};
  renderer_clear_color(&game->renderer, color);
  renderer_present(&game->renderer);

  static float dr = 0.3f;
  static float dg = 0.4f;
  static float db = 0.5f;
  
  color[0] += dr * delta;
  color[1] += dg * delta;
  color[2] += db * delta;
  
  // Smooth bouncing at boundaries
  if (color[0] > 1.0f) { color[0] = 1.0f; dr = -dr; }
  if (color[0] < 0.0f) { color[0] = 0.0f; dr = -dr; }
  
  if (color[1] > 1.0f) { color[1] = 1.0f; dg = -dg; }
  if (color[1] < 0.0f) { color[1] = 0.0f; dg = -dg; }

  if (color[2] > 1.0f) { color[2] = 1.0f; db = -db; }
  if (color[2] < 0.0f) { color[2] = 0.0f; db = -db; }
}

void game_fixed_update(Game *game, float delta) {
  
}

void game_destroy(Game *game) {
  if (!game) {
    return;
  }
}
