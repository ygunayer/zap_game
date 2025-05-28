#ifndef _GAME_H_
#define _GAME_H_

#define TICKS_PER_SECOND 1000000

#define DEFAULT_WINDOW_WIDTH 1366
#define DEFAULT_WINDOW_HEIGHT 768
#define DEFAULT_TARGET_FPS 144
#define DEFAULT_FIXED_FPS 60

#define TARGET_FPS_VSYNC -1
#define TARGET_FPS_UNLIMITED 0

#if defined(PLATFORM_WINDOWS)
  #include <Windows.h>
#elif defined(PLATFORM_MACOS)
  #include "stdafx.h"
#endif

#include "renderer.h"

typedef uint64_t tick_t;

typedef struct Game {
  tick_t total_ticks;
  tick_t last_tick;

  tick_t tick_acc;
  tick_t ticks_per_frame;
  uint64_t num_updates;
  int target_fps;

  tick_t fixed_tick_acc;
  tick_t fixed_ticks_per_frame;
  uint64_t num_fixed_updates;
  int fixed_fps;

  Renderer renderer;

#if defined(PLATFORM_WINDOWS)
  HWND hwnd;
#endif

  bool inited;
} Game;

bool game_init(Game* game, zap_window_t window);
void game_tick(Game* game, tick_t ticks);
void game_set_fixed_fps(Game* game, int target_fps);
void game_set_target_fps(Game* game, int target_fps);

void game_update(Game* game, float delta);
void game_fixed_update(Game* game, float delta);

void game_destroy(Game* game);

#endif // _GAME_H_
