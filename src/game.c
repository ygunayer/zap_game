#include "game.h"
#include <assert.h>
#include <stdio.h>

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

}

void game_fixed_update(Game *game, float delta) {
  
}

void game_destroy(Game *game) {
  if (!game) {
    return;
  }
}
