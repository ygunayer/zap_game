#define ZAP_IMPL
#include <zap.h>
#undef ZAP_IMPL

#include "../game.h"

static Game game;

void on_window_update(zap_window_t window) {
  (void)window;
  game_tick(&game, zap_get_ticks());
}

bool create_main_window(void) {
  return zap_window_create((zap_window_options_t) {
    .width = 1366,
    .height = 768,
    .title = "game",
    .on_update = on_window_update,
  });
}

bool on_before_destroy(void) {
  game_destroy(&game);
  return true;
}

void on_event(zap_event_t event) {
  (void)event;
}

bool on_after_init(zap_options_t opts) {
  (void)opts;
  zap_window_t window = create_main_window();
  if (!window) {
    return false;
  }

  return game_init(&game, window);
}

int main(int argc, const char** argv) {
  zap_options_t options = {
    .on_after_init = on_after_init,
    .on_event = on_event,
    .on_before_destroy = on_before_destroy,
  };
  return zap_main(argc, argv, options);
}
