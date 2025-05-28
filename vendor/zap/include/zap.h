#ifndef _ZAP_H_
#define _ZAP_H_

// #define ZAP_IMPL

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Platform-specific imports
#if defined(_WIN32) || defined(_WIN64)
  #define _ZAP_WINDOWS
  #include <Windows.h>
#elif defined(__linux__)
  #define _ZAP_X11
  #include <X11/Xlib.h>
  #include <X11/Xatom.h>
  #include <X11/Xutil.h>
  #include <X11/keysymdef.h>
  #include <X11/extensions/Xrandr.h>
#elif defined(__APPLE__)
  #define _ZAP_MACOS

  #include <Cocoa/Cocoa.h>
#endif

// User and built-in defines
#define ZAP_TICKS_PER_SECOND 1000000

#ifndef ZAP_API
  #define ZAP_API
#endif

#ifndef _ZAP_INTERNAL
  #if defined(__GNUC__) || defined(__clang__)
    #define _ZAP_INTERNAL __attribute__((unused)) static
  #else
    #define _ZAP_INTERNAL static
  #endif
#endif

#if defined(_ZAP_WINDOWS)
  #ifdef ZAP_WINDOWS_WNDCLASS_NAME
    #define _ZAP_WINDOWS_WNDCLASS ZAP_WINDOWS_WNDCLASS_NAME
  #else
    #define _ZAP_WINDOWS_WNDCLASS "zapWndClass"
  #endif
#endif

// Forward Declarations
typedef struct zap_options_t zap_options_t;
typedef struct zap_window_options_t zap_window_options_t;

// API - Typedefs and defines
typedef uint32_t zap_window_t;
typedef uint32_t zap_display_t;
typedef uint64_t zap_tick_t;

typedef enum zap_window_display_mode_t {
  ZAP_DISPLAY_MODE_INVALID = -1,
  ZAP_DISPLAY_MODE_NORMAL = 0,
  ZAP_DISPLAY_MODE_MAXIMIZED,
  ZAP_DISPLAY_MODE_FULLSCREEN,
  ZAP_DISPLAY_MODE_BORDERLESS_FULLSCREEN,
} zap_window_display_mode_t;

typedef enum zap_window_position_type_t {
  ZAP_WINDOW_POSITION_AUTO,
  ZAP_WINDOW_POSITION_CUSTOM,
  ZAP_WINDOW_POSITION_CENTERED,
} zap_window_position_type_t;

typedef enum zap_event_type_t {
  ZAP_EVENT_KEY_INVALID = 0,
  ZAP_EVENT_KEY_DOWN,
  ZAP_EVENT_KEY_UP,
  ZAP_EVENT_MOUSE_ENTERED,
  ZAP_EVENT_MOUSE_LEFT,
  ZAP_EVENT_MOUSE_MOVED,
  ZAP_EVENT_MOUSE_BUTTON_CLICKED,
  ZAP_EVENT_MOUSE_BUTTON_UP,
  ZAP_EVENT_MOUSE_BUTTON_DOWN,
  ZAP_EVENT_WINDOW_RESIZED,
  ZAP_EVENT_WINDOW_MOVED,
  ZAP_EVENT_WINDOW_FOCUSED,
  ZAP_EVENT_WINDOW_UNFOCUSED,
  ZAP_EVENT_DISPLAY_MODE_CHANGED,
  ZAP_EVENT_FILE_DROP_STARTED,
  ZAP_EVENT_FILE_DROPPED,
  ZAP_EVENT_FILE_DROP_ENDED,
  ZAP_EVENT_TYPE_COUNT,
} zap_event_type_t;

typedef enum zap_keycode_t {
  ZAP_KEYCODE_INVALID          = 0,
  ZAP_KEYCODE_SPACE            = 32,
  ZAP_KEYCODE_APOSTROPHE       = 39,  /* ' */
  ZAP_KEYCODE_COMMA            = 44,  /* , */
  ZAP_KEYCODE_MINUS            = 45,  /* - */
  ZAP_KEYCODE_PERIOD           = 46,  /* . */
  ZAP_KEYCODE_SLASH            = 47,  /* / */
  ZAP_KEYCODE_0                = 48,
  ZAP_KEYCODE_1                = 49,
  ZAP_KEYCODE_2                = 50,
  ZAP_KEYCODE_3                = 51,
  ZAP_KEYCODE_4                = 52,
  ZAP_KEYCODE_5                = 53,
  ZAP_KEYCODE_6                = 54,
  ZAP_KEYCODE_7                = 55,
  ZAP_KEYCODE_8                = 56,
  ZAP_KEYCODE_9                = 57,
  ZAP_KEYCODE_SEMICOLON        = 59,  /* ; */
  ZAP_KEYCODE_EQUAL            = 61,  /* = */
  ZAP_KEYCODE_A                = 65,
  ZAP_KEYCODE_B                = 66,
  ZAP_KEYCODE_C                = 67,
  ZAP_KEYCODE_D                = 68,
  ZAP_KEYCODE_E                = 69,
  ZAP_KEYCODE_F                = 70,
  ZAP_KEYCODE_G                = 71,
  ZAP_KEYCODE_H                = 72,
  ZAP_KEYCODE_I                = 73,
  ZAP_KEYCODE_J                = 74,
  ZAP_KEYCODE_K                = 75,
  ZAP_KEYCODE_L                = 76,
  ZAP_KEYCODE_M                = 77,
  ZAP_KEYCODE_N                = 78,
  ZAP_KEYCODE_O                = 79,
  ZAP_KEYCODE_P                = 80,
  ZAP_KEYCODE_Q                = 81,
  ZAP_KEYCODE_R                = 82,
  ZAP_KEYCODE_S                = 83,
  ZAP_KEYCODE_T                = 84,
  ZAP_KEYCODE_U                = 85,
  ZAP_KEYCODE_V                = 86,
  ZAP_KEYCODE_W                = 87,
  ZAP_KEYCODE_X                = 88,
  ZAP_KEYCODE_Y                = 89,
  ZAP_KEYCODE_Z                = 90,
  ZAP_KEYCODE_LEFT_BRACKET     = 91,  /* [ */
  ZAP_KEYCODE_BACKSLASH        = 92,  /* \ */
  ZAP_KEYCODE_RIGHT_BRACKET    = 93,  /* ] */
  ZAP_KEYCODE_GRAVE_ACCENT     = 96,  /* ` */
  ZAP_KEYCODE_WORLD_1          = 161, /* non-US #1 */
  ZAP_KEYCODE_WORLD_2          = 162, /* non-US #2 */
  ZAP_KEYCODE_ESCAPE           = 256,
  ZAP_KEYCODE_ENTER            = 257,
  ZAP_KEYCODE_TAB              = 258,
  ZAP_KEYCODE_BACKSPACE        = 259,
  ZAP_KEYCODE_INSERT           = 260,
  ZAP_KEYCODE_DELETE           = 261,
  ZAP_KEYCODE_RIGHT            = 262,
  ZAP_KEYCODE_LEFT             = 263,
  ZAP_KEYCODE_DOWN             = 264,
  ZAP_KEYCODE_UP               = 265,
  ZAP_KEYCODE_PAGE_UP          = 266,
  ZAP_KEYCODE_PAGE_DOWN        = 267,
  ZAP_KEYCODE_HOME             = 268,
  ZAP_KEYCODE_END              = 269,
  ZAP_KEYCODE_CAPS_LOCK        = 280,
  ZAP_KEYCODE_SCROLL_LOCK      = 281,
  ZAP_KEYCODE_NUM_LOCK         = 282,
  ZAP_KEYCODE_PRINT_SCREEN     = 283,
  ZAP_KEYCODE_PAUSE            = 284,
  ZAP_KEYCODE_F1               = 290,
  ZAP_KEYCODE_F2               = 291,
  ZAP_KEYCODE_F3               = 292,
  ZAP_KEYCODE_F4               = 293,
  ZAP_KEYCODE_F5               = 294,
  ZAP_KEYCODE_F6               = 295,
  ZAP_KEYCODE_F7               = 296,
  ZAP_KEYCODE_F8               = 297,
  ZAP_KEYCODE_F9               = 298,
  ZAP_KEYCODE_F10              = 299,
  ZAP_KEYCODE_F11              = 300,
  ZAP_KEYCODE_F12              = 301,
  ZAP_KEYCODE_F13              = 302,
  ZAP_KEYCODE_F14              = 303,
  ZAP_KEYCODE_F15              = 304,
  ZAP_KEYCODE_F16              = 305,
  ZAP_KEYCODE_F17              = 306,
  ZAP_KEYCODE_F18              = 307,
  ZAP_KEYCODE_F19              = 308,
  ZAP_KEYCODE_F20              = 309,
  ZAP_KEYCODE_F21              = 310,
  ZAP_KEYCODE_F22              = 311,
  ZAP_KEYCODE_F23              = 312,
  ZAP_KEYCODE_F24              = 313,
  ZAP_KEYCODE_F25              = 314,
  ZAP_KEYCODE_KP_0             = 320,
  ZAP_KEYCODE_KP_1             = 321,
  ZAP_KEYCODE_KP_2             = 322,
  ZAP_KEYCODE_KP_3             = 323,
  ZAP_KEYCODE_KP_4             = 324,
  ZAP_KEYCODE_KP_5             = 325,
  ZAP_KEYCODE_KP_6             = 326,
  ZAP_KEYCODE_KP_7             = 327,
  ZAP_KEYCODE_KP_8             = 328,
  ZAP_KEYCODE_KP_9             = 329,
  ZAP_KEYCODE_KP_DECIMAL       = 330,
  ZAP_KEYCODE_KP_DIVIDE        = 331,
  ZAP_KEYCODE_KP_MULTIPLY      = 332,
  ZAP_KEYCODE_KP_SUBTRACT      = 333,
  ZAP_KEYCODE_KP_ADD           = 334,
  ZAP_KEYCODE_KP_ENTER         = 335,
  ZAP_KEYCODE_KP_EQUAL         = 336,
  ZAP_KEYCODE_LEFT_SHIFT       = 340,
  ZAP_KEYCODE_LEFT_CONTROL     = 341,
  ZAP_KEYCODE_LEFT_ALT         = 342,
  ZAP_KEYCODE_LEFT_SUPER       = 343,
  ZAP_KEYCODE_RIGHT_SHIFT      = 344,
  ZAP_KEYCODE_RIGHT_CONTROL    = 345,
  ZAP_KEYCODE_RIGHT_ALT        = 346,
  ZAP_KEYCODE_RIGHT_SUPER      = 347,
  ZAP_KEYCODE_MENU             = 348,
} zap_keycode_t;

typedef enum zap_keymod_t {
  ZAP_KEYMOD_SHIFT = (1 << 1),
  ZAP_KEYMOD_CTRL  = (1 << 2),
  ZAP_KEYMOD_ALT   = (1 << 3),
  ZAP_KEYMOD_META  = (1 << 4),
} zap_keymod_t;

typedef enum zap_mbutton_t {
  ZAP_MBUTTON_LEFT   = (1 << 1),
  ZAP_MBUTTON_RIGHT  = (1 << 2),
  ZAP_MBUTTON_MIDDLE = (1 << 3),
} zap_mbutton_t;

typedef struct {
  int x;
  int y;
  int width;
  int height;
} zap_recti_t;

typedef struct zap_event_t {
  zap_event_type_t type;
  zap_window_t window;
  zap_keycode_t keycode;
  zap_keymod_t keymod;
  bool key_repeat;
  const char* filename;
} zap_event_t;

typedef struct zap_display_info_t {
  zap_display_t id;
  zap_window_display_mode_t display_mode;
  zap_recti_t rect;
  uint32_t refresh_rate;
} zap_display_info_t;

typedef bool (*ZapInitCallback)(zap_options_t options);
typedef bool (*ZapDestroyCallback)(void);
typedef void (*ZapEventCallback)(zap_event_t event);

typedef void (*ZapWindowCreateCallback)(zap_window_t window, zap_window_options_t options);
typedef void (*ZapWindowUpdateCallback)(zap_window_t window);
typedef bool (*ZapWindowCloseCallback)(zap_window_t window);
typedef void (*ZapWindowDestroyCallback)(zap_window_t window);

typedef struct zap_options_t {
  void* user_data;
  ZapInitCallback on_after_init;
  ZapDestroyCallback on_before_destroy;
  ZapEventCallback on_event;
} zap_options_t;

typedef struct zap_window_options_t {
  int x;
  int y;
  int width;
  int height;
  zap_window_position_type_t position;
  zap_window_display_mode_t display_mode;
  char* title;
  void* user_data;
  ZapWindowCreateCallback on_after_create;
  ZapWindowUpdateCallback on_update;
  ZapWindowCloseCallback on_before_close;
  ZapWindowDestroyCallback on_before_destroy;
} zap_window_options_t;

// API - Functions
ZAP_API char* zap_get_last_error(void);

ZAP_API int zap_main(int argc, const char** argv, zap_options_t options);

// Returns the time elapsed since zap was initialized in microseconds.
ZAP_API zap_tick_t zap_get_ticks(void);

ZAP_API bool zap_init(zap_options_t options);
ZAP_API void zap_destroy(void);
ZAP_API void zap_run_loop(void);

ZAP_API void zap_request_exit(void);
ZAP_API void zap_set_user_data(void* user_data);
ZAP_API void* zap_get_user_data(void);

ZAP_API zap_display_t zap_display_get_primary(void);
ZAP_API bool zap_display_get_info(zap_display_t display, zap_display_info_t* pinfo);

ZAP_API zap_window_t zap_window_create(zap_window_options_t options);
ZAP_API zap_window_display_mode_t zap_window_get_display_mode(zap_window_t window);
ZAP_API void zap_window_set_display_mode(zap_window_t window, zap_window_display_mode_t display_mode);
ZAP_API void zap_window_center_on_screen(zap_window_t window);
ZAP_API void zap_window_request_close(zap_window_t window);
ZAP_API void zap_window_set_title(zap_window_t window, const char* new_title, size_t len);
ZAP_API zap_display_t zap_window_get_display(zap_window_t window);
ZAP_API bool zap_window_get_position(zap_window_t window, int* x, int* y);
ZAP_API bool zap_window_get_size(zap_window_t window, int* width, int* height);

ZAP_API void zap_window_set_user_data(zap_window_t window, void* user_data);
ZAP_API void* zap_window_get_user_data(zap_window_t window);

#if defined(_ZAP_WINDOWS)
ZAP_API HWND zap_window_get_hwnd(zap_window_t window);
#endif

// Internal implementation
#if defined(ZAP_IMPL)
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define _ZAP_WINDOWS_FOREACH(x) \
  do { \
    assert(ZAP.inited); \
    for (size_t i = 0; i < ZAP.window_count; ++i) { \
      _zap_window_entry_t* it = &ZAP.windows[i]; \
      x \
    } \
  } while (0)

#define _ZAP_DISPLAYS_FOREACH(x) \
  do { \
    assert(ZAP.inited); \
    for (size_t i = 0; i < ZAP.display_count; ++i) { \
      _zap_display_entry_t* it = &ZAP.displays[i]; \
      x \
    } \
  } while (0)

typedef struct {
  zap_window_t id;
  zap_recti_t rect;
  zap_recti_t previous_rect;
  zap_window_display_mode_t display_mode;
  bool close_requested;
#if defined(_ZAP_WINDOWS)
  HWND hwnd;
#elif defined(_ZAP_X11)
  Window xwindow;
#elif defined(_ZAP_MACOS)
  NSWindow* nswindow;
#endif
  ZapWindowCreateCallback on_after_create;
  ZapWindowUpdateCallback on_update;
  ZapWindowCloseCallback on_before_close;
  ZapWindowDestroyCallback on_before_destroy;
  void* user_data;
} _zap_window_entry_t;

typedef struct _zap_display_entry_t {
  zap_display_info_t info;
#if defined(_ZAP_WINDOWS)
  wchar_t win32_device_name[32];
#elif defined(_ZAP_X11)
  char* x11_display_name;
#elif defined(_ZAP_MACOS)
  NSNumber* nsscreen_number;
#endif
} _zap_display_entry_t;

static struct ZAP {
  zap_window_t next_window_id;
  _zap_window_entry_t* windows;
  size_t window_count;
  size_t window_cap;

  zap_window_t next_display_id;
  _zap_display_entry_t* displays;
  size_t display_count;
  size_t display_cap;

  _zap_display_entry_t* primary_display;

#if defined(_ZAP_WINDOWS)
  HINSTANCE hinstance;
  WNDCLASSEX wndclass;
  LARGE_INTEGER qpstart;
  LARGE_INTEGER qpfreq;
#elif defined(_ZAP_X11)
  Atom xa_wm_delete_window;
  Atom xa_window_id;
  Window xroot_window;
  Display* xdisplay;
#elif defined(_ZAP_MACOS)
  NSAutoreleasePool* nspool;
  NSApplication* nsapp;
#endif

  zap_keycode_t keycodes[512];
  bool inited;
  bool init_displays_loaded;
  ZapDestroyCallback on_before_destroy;
  ZapEventCallback on_event;
  void* user_data;
} ZAP;

static char* _zap_last_error;
_ZAP_INTERNAL inline void _zap_close_pending_windows(void);
_ZAP_INTERNAL bool _zap_refresh_displays(void);
_ZAP_INTERNAL void _zap_window_destroy(_zap_window_entry_t* window);
_ZAP_INTERNAL void _zap_window_move_to(_zap_window_entry_t* window, int x, int y, int w, int h);
_ZAP_INTERNAL void _zap_window_set_display_mode(_zap_window_entry_t* window, zap_window_display_mode_t display_mode);
_ZAP_INTERNAL void _zap_window_center_on_screen(_zap_window_entry_t* window);
_ZAP_INTERNAL inline _zap_window_entry_t* _zap_window_find(zap_window_t id);
_ZAP_INTERNAL inline void _zap_window_refresh_size(_zap_window_entry_t* window);
_ZAP_INTERNAL _zap_display_entry_t* _zap_window_get_display(_zap_window_entry_t* window);
_ZAP_INTERNAL void _zap_display_destroy(_zap_display_entry_t* display);
_ZAP_INTERNAL _zap_display_entry_t* _zap_display_append_new(void);

#if defined(_ZAP_WINDOWS)
_ZAP_INTERNAL bool _zap_windows_init(void);
LRESULT CALLBACK ZapWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
_ZAP_INTERNAL bool _zap_windows_refresh_displays(void);
_ZAP_INTERNAL bool _zap_windows_upsert_display(const DISPLAY_DEVICEW *display_device, const DEVMODEW *device_mode);
#elif defined(_ZAP_X11)
_ZAP_INTERNAL bool _zap_x11_init(void);
_ZAP_INTERNAL void _zap_x11_handle_events(void);
_ZAP_INTERNAL bool _zap_x11_refresh_displays(void);
_ZAP_INTERNAL void _zap_x11_upsert_display(XRROutputInfo* output_info, XRRCrtcInfo* crtc_info);
_ZAP_INTERNAL zap_window_t _zap_x11_get_window(Window window);
_ZAP_INTERNAL _zap_window_entry_t* _zap_x11_find_window_entry(Window window);
#elif defined(_ZAP_MACOS)
_ZAP_INTERNAL bool _zap_macos_init(void);
_ZAP_INTERNAL void _zap_macos_destroy(void);
_ZAP_INTERNAL bool _zap_macos_refresh_displays(void);
#endif

ZAP_API int zap_main(int argc, const char** argv, zap_options_t options) {
  (void)argc;
  (void)argv;
  if (!zap_init(options)) {
    return 1;
  }
  zap_run_loop();
  zap_destroy();
  return 0;
}

ZAP_API zap_tick_t zap_get_ticks(void) {
#if defined(_ZAP_WINDOWS)
  LARGE_INTEGER ticks;
  assert(QueryPerformanceCounter(&ticks));
  LARGE_INTEGER elapsed;
  elapsed.QuadPart = ticks.QuadPart - ZAP.qpstart.QuadPart;
  elapsed.QuadPart *= ZAP_TICKS_PER_SECOND;
  elapsed.QuadPart /= ZAP.qpfreq.QuadPart;
  return elapsed.QuadPart;
#endif
  return 0;
}

ZAP_API bool zap_init(zap_options_t options) {
  if (ZAP.inited) {
    return false;
  }

  if (options.user_data) {
    ZAP.user_data = options.user_data;
  }

  ZAP.on_before_destroy = options.on_before_destroy;
  ZAP.on_event = options.on_event;

  ZAP.next_window_id = 1;
  ZAP.window_cap = 16;
  ZAP.windows = (_zap_window_entry_t*)malloc(sizeof(_zap_window_entry_t) * ZAP.window_cap);
  memset(ZAP.windows, 0, sizeof(_zap_window_entry_t) * ZAP.window_cap);

  ZAP.next_display_id = 1;
  ZAP.display_cap = 16;
  ZAP.displays = (_zap_display_entry_t*)malloc(sizeof(_zap_display_entry_t) * ZAP.display_cap);
  memset(ZAP.displays, 0, sizeof(_zap_display_entry_t) * ZAP.display_cap);

#if defined(_ZAP_WINDOWS)
  if (!_zap_windows_init()) {
    return false;
  }
#elif defined(_ZAP_X11)
  if (!_zap_x11_init()) {
    return false;
  }
#elif defined(_ZAP_MACOS)
  if (!_zap_macos_init()) {
    return false;
  }
#endif

  ZAP.inited = true;
  if (!_zap_refresh_displays()) {
    return false;
  }

  if (options.on_after_init && !options.on_after_init(options)) {
    ZAP.inited = false;
    return false;
  }

  return true;
}

ZAP_API void zap_destroy(void) {
  if (ZAP.on_before_destroy) {
    ZAP.on_before_destroy();
  }

  if (ZAP.windows) {
    _ZAP_WINDOWS_FOREACH({
      _zap_window_destroy(it);
    });
    free(ZAP.windows);
    ZAP.window_count = 0;
    ZAP.windows = NULL;
  }

  if (ZAP.displays) {
    _ZAP_DISPLAYS_FOREACH({
      _zap_display_destroy(it);
    });
    free(ZAP.displays);
    ZAP.display_count = 0;
    ZAP.displays = NULL;
  }

#if defined(_ZAP_WINDOWS)
  // TODO cleanup
#elif defined(_ZAP_X11)
  if (ZAP.xdisplay) {
    XCloseDisplay(ZAP.xdisplay);
    ZAP.xdisplay = NULL;
  }
#endif
}

ZAP_API void zap_run_loop(void) {
  assert(ZAP.inited);

#if defined(_ZAP_WINDOWS)
  MSG msg;
#endif

  while (ZAP.window_count > 0) {
#if defined(_ZAP_X11)
    _zap_x11_handle_events();
#elif defined(_ZAP_WINDOWS)
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
#endif

    _ZAP_WINDOWS_FOREACH({
      if (it->on_update) {
        it->on_update(it->id);
      }
    });

    _zap_close_pending_windows();
  }
}

ZAP_API void zap_request_exit(void) {
  _ZAP_WINDOWS_FOREACH(it->close_requested = true;);
}

ZAP_API void zap_set_user_data(void* user_data) {
  assert(ZAP.inited);
  ZAP.user_data = user_data;
}

ZAP_API void* zap_get_user_data(void) {
  assert(ZAP.user_data);
  return ZAP.user_data;
}

ZAP_API zap_display_t zap_display_get_primary(void) {
  if (!ZAP.primary_display) {
    return 0;
  }
  return ZAP.primary_display->info.id;
}

ZAP_API bool zap_display_get_info(zap_display_t display, zap_display_info_t* pinfo) {
  _ZAP_DISPLAYS_FOREACH({
    if (it->info.id == display) {
      *pinfo = it->info;
      return true;
    };
  });
  return false;
}

ZAP_API zap_window_t zap_window_create(zap_window_options_t options) {
  _zap_window_entry_t window = {
    .id = ZAP.next_window_id,
    .rect = {
      .width = options.width,
      .height = options.height,
    },
    .on_after_create = options.on_after_create,
    .on_update = options.on_update,
    .on_before_close = options.on_before_close,
    .on_before_destroy = options.on_before_destroy,
    .user_data = options.user_data,
  };

  char* title = options.title ? options.title : (char*)"zap";

#if defined(_ZAP_WINDOWS)
  assert(ZAP.hinstance);

  HWND hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    _ZAP_WINDOWS_WNDCLASS,
    title,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    NULL,
    NULL,
    ZAP.hinstance,
    NULL
  );

  if (!hwnd) {
    return 0;
  }

  SetWindowLongPtrW(hwnd, GWLP_USERDATA, window.id);
  window.hwnd = hwnd;
#elif defined(_ZAP_X11)
  assert(ZAP.xdisplay);

  XSetWindowAttributes attrs = {0};
  attrs.background_pixel = 0x00000000;
  attrs.event_mask =
    StructureNotifyMask |
    KeyPressMask |
    KeyReleaseMask |
    ExposureMask;

  Window xwindow = XCreateWindow(
    ZAP.xdisplay,
    ZAP.xroot_window,
    0, 0,
    options.width, options.height,
    InputOutput,
    CopyFromParent,
    CopyFromParent,
    CopyFromParent,
    CWBackPixel | CWEventMask,
    &attrs
  );

  window.xwindow = xwindow;

  XStoreName(ZAP.xdisplay, xwindow, title);
  XSetWMProtocols(ZAP.xdisplay, xwindow, &ZAP.xa_wm_delete_window, 1);

  unsigned char window_id_prop_val[1] = {0};
  window_id_prop_val[0] = window.id;

  XChangeProperty(
    ZAP.xdisplay,
    xwindow,
    ZAP.xa_window_id,
    XA_INTEGER,
    sizeof(zap_window_t) * 8,
    PropModeReplace,
    window_id_prop_val,
    1
  );

  XFlush(ZAP.xdisplay);
#elif defined(_ZAP_MACOS)
  NSWindow* nswindow = [[NSWindow alloc]
      initWithContentRect:NSZeroRect
      styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskFullSizeContentView)
      backing:NSBackingStoreBuffered
      defer:false
  ];
  if (!nswindow) {
    return false;
  }

  @autoreleasepool {
    NSString* titlestr = [NSString stringWithUTF8String:title];
    [nswindow setTitle:titlestr];
  }

  window.nswindow = nswindow;
#endif

  if (ZAP.window_count >= ZAP.window_cap) {
    while (ZAP.window_count >= ZAP.window_cap) {
      ZAP.window_cap *= 2;
    }
    ZAP.windows = (_zap_window_entry_t*)realloc(ZAP.windows, sizeof(_zap_window_entry_t) * ZAP.window_cap);
  }

  ZAP.windows[ZAP.window_count] = window;
  ZAP.next_window_id += 1;
  ZAP.window_count += 1;

  switch (options.display_mode) {
    case ZAP_DISPLAY_MODE_NORMAL: {
      switch (options.position) {
        case ZAP_WINDOW_POSITION_AUTO: {
#if defined(_ZAP_WINDOWS)
          RECT rect = {0};
          if (GetWindowRect(hwnd, &rect)) {
            _zap_window_move_to(&window, rect.left, rect.top, options.width, options.height);
          }
#elif defined(_ZAP_X11)
          XSizeHints size_hints = {
            .width = options.width,
            .height = options.height,
            .flags = PSize,
          };
          XSetWMNormalHints(ZAP.xdisplay, xwindow, &size_hints);
#endif
        } break;

        case ZAP_WINDOW_POSITION_CUSTOM:
          _zap_window_move_to(&window, options.x, options.y, options.width, options.height);
          break;

        case ZAP_WINDOW_POSITION_CENTERED:
          _zap_window_center_on_screen(&window);
          break;
      }
    } break;

    default: {
      _zap_window_set_display_mode(&window, options.display_mode);
    } break;
  }

#if defined(_ZAP_WINDOWS)
  ShowWindow(hwnd, SW_SHOW);
  DragAcceptFiles(hwnd, TRUE);
#elif defined(_ZAP_X11)
  XMapWindow(ZAP.xdisplay, xwindow);
#elif defined(_ZAP_MACOS)
  [nswindow makeKeyAndOrderFront:NULL];
#endif

  return window.id;
}

ZAP_API zap_window_display_mode_t zap_window_get_display_mode(zap_window_t window) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (!win) {
    return ZAP_DISPLAY_MODE_INVALID;
  }
  return win->display_mode;
}

ZAP_API void zap_window_set_display_mode(zap_window_t window, zap_window_display_mode_t display_mode) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (win) {
    _zap_window_set_display_mode(win, display_mode);
  }
}


ZAP_API void zap_window_center_on_screen(zap_window_t window) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (win) {
    _zap_window_center_on_screen(win);
  }
}

ZAP_API void zap_window_request_close(zap_window_t window) {
  assert(window != 0);
  _zap_window_entry_t* win = _zap_window_find(window);

  if (!win || win->close_requested) {
    return;
  }

  if (win->on_before_close && !win->on_before_close(window)) {
    return;
  }

  win->close_requested = true;
}

ZAP_API void zap_window_set_title(zap_window_t window, const char* new_title, size_t len) {
  char* buf = (char*)malloc(sizeof(char) * len + 1);
#if defined(_ZAP_WINDOWS)
  strncpy_s(buf, sizeof(char) * (len + 1), new_title, len);
  SetWindowTextW(zap_window_get_hwnd(window), TEXT((const unsigned short*) buf));
#endif
  free(buf);
}

ZAP_API zap_display_t zap_window_get_display(zap_window_t window) {
  (void)window;
  _zap_window_entry_t* entry = _zap_window_find(window);
  if (!entry) {
    return 0;
  }

  _zap_display_entry_t* display = _zap_window_get_display(entry);
  if (!display) {
    return 0;
  }

  return display->info.id;
}

ZAP_API bool zap_window_get_position(zap_window_t window, int* x, int* y) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (win) {
    *x = win->rect.x;
    *y = win->rect.y;
    return true;
  }
  return false;
}

ZAP_API bool zap_window_get_size(zap_window_t window, int* width, int* height) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (win) {
    *width = win->rect.width;
    *height = win->rect.height;
    return true;
  }
  return false;
}

ZAP_API void zap_window_set_user_data(zap_window_t window, void* user_data) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (win) {
    win->user_data = user_data;
  }
}

ZAP_API void* zap_window_get_user_data(zap_window_t window) {
  _zap_window_entry_t* win = _zap_window_find(window);
  if (!win) {
    return NULL;
  }
  return win->user_data;
}

#if defined(_ZAP_WINDOWS)
ZAP_API HWND zap_window_get_hwnd(zap_window_t window) {
  _zap_window_entry_t* win = _zap_window_find(window);
  return !win ? NULL : win->hwnd;
}
#endif

// Internal implementation
_ZAP_INTERNAL void _zap_window_move_to(_zap_window_entry_t* window, int x, int y, int w, int h) {
  assert(window);
#if defined(_ZAP_WINDOWS)
  if (window->hwnd) {
    MoveWindow(window->hwnd, x, y, w, h, true);

    RECT new_rect = {0};
    if (GetWindowRect(window->hwnd, &new_rect)) {
      window->previous_rect = window->rect;

      zap_recti_t* rect = &window->rect;
      rect->x = new_rect.left;
      rect->y = new_rect.top;
      rect->width = new_rect.right - new_rect.left;
      rect->height = new_rect.bottom - new_rect.top;
    }
  }
#elif defined(_ZAP_X11)
  assert(ZAP.xdisplay);
  XMoveResizeWindow(ZAP.xdisplay, window->xwindow, x, y, w, h);
#elif defined(_ZAP_MACOS)
  NSPoint nspos = {
    .x = x,
    .y = y,
  };
  NSSize nssize = {
    .width = w,
    .height = h,
  };
  [window->nswindow setFrameTopLeftPoint:nspos];
  [window->nswindow setContentSize:nssize];
#endif
}

_ZAP_INTERNAL void _zap_window_set_display_mode(_zap_window_entry_t* window, zap_window_display_mode_t display_mode) {
  assert(window);
  if (display_mode == ZAP_DISPLAY_MODE_INVALID) {
    return;
  }

  _zap_display_entry_t* display = _zap_window_get_display(window);
  assert(display);

  switch (display_mode) {
    case ZAP_DISPLAY_MODE_FULLSCREEN: {

    } break;

    case ZAP_DISPLAY_MODE_BORDERLESS_FULLSCREEN: {
      window->previous_rect = window->rect;
#if defined(_ZAP_WINDOWS)
      if (window->hwnd) {
        SetWindowLong(window->hwnd, GWL_STYLE, 0);
        zap_recti_t display_rect = display->info.rect;
        SetWindowPos(window->hwnd, HWND_TOP, display_rect.x, display_rect.y, display_rect.width, display_rect.height, SWP_FRAMECHANGED);
      }
#endif
    } break;

    case ZAP_DISPLAY_MODE_MAXIMIZED: {
      // TODO implement this
    } break;

    case ZAP_DISPLAY_MODE_NORMAL: {
      // TODO implement this
    } break;

    default:
      break;
  }

  window->display_mode = display_mode;
}

_ZAP_INTERNAL inline void _zap_close_pending_windows(void) {
  _ZAP_WINDOWS_FOREACH({
    if (!it->close_requested) {
      continue;
    }

    _zap_window_destroy(it);
    for (size_t j = i; j < (ZAP.window_count - 1); ++j) {
      ZAP.windows[j] = ZAP.windows[j + 1];
    }
    ZAP.window_count -= 1;
  });
}

_ZAP_INTERNAL bool _zap_refresh_displays(void) {
  assert(ZAP.inited);
  assert(ZAP.displays);

#if defined(_ZAP_WINDOWS)
  if (!_zap_windows_refresh_displays()) {
    return false;
  }
#elif defined(_ZAP_X11)
  if (!_zap_x11_refresh_displays()) {
    return false;
  }
#elif defined(_ZAP_MACOS)
  if (!_zap_macos_refresh_displays()) {
    return false;
  }
#endif

  return true;
}

_ZAP_INTERNAL void _zap_window_destroy(_zap_window_entry_t* window) {
  if (!window) {
    return;
  }

  if (window->on_before_destroy) {
    window->on_before_destroy(window->id);
  }

#if defined(_ZAP_WINDOWS)
  if (window->hwnd) {
    DestroyWindow(window->hwnd);
  }
#elif defined(_ZAP_X11)
  XDestroyWindow(ZAP.xdisplay, window->xwindow);
#endif
}

_ZAP_INTERNAL void _zap_window_center_on_screen(_zap_window_entry_t *window) {
  assert(window);

#if defined(_ZAP_MACOS)
  if (window->nswindow) {
    [window->nswindow center];
  }
  return;
#endif

  _zap_display_entry_t* display = _zap_window_get_display(window);
  if (display) {
    int x = (display->info.rect.width - window->rect.width) / 2;
    int y = (display->info.rect.height - window->rect.height) / 2;
    _zap_window_move_to(window, x, y, window->rect.width, window->rect.height);
  }
}

_ZAP_INTERNAL inline _zap_window_entry_t* _zap_window_find(zap_window_t window) {
  _ZAP_WINDOWS_FOREACH({
    if (it->id == window) {
      return it;
    }
  });
  return NULL;
}

_ZAP_INTERNAL inline void _zap_window_refresh_size(_zap_window_entry_t* window) {
  assert(window);
  zap_recti_t* rect = &window->rect;
#if defined(_ZAP_WINDOWS)
  RECT winrect = {0};
  if (window->hwnd && GetWindowRect(window->hwnd, &winrect)) {
    rect->x = winrect.left;
    rect->y = winrect.top;
    rect->width = winrect.right - winrect.left;
    rect->height = winrect.bottom - winrect.top;
  }
#elif defined(_ZAP_X11)
  XWindowAttributes attrs = {0};
  XGetWindowAttributes(ZAP.xdisplay, window->xwindow, &attrs);
  rect->x = attrs.x;
  rect->y = attrs.y;
  rect->width = attrs.width;
  rect->height = attrs.height;
#elif defined(_ZAP_MACOS)
  NSRect nsrect = window->nswindow.frame;
  rect->x = nsrect.origin.x;
  rect->y = nsrect.origin.y;
  rect->width = nsrect.size.width;
  rect->height = nsrect.size.height;
#endif
}

_ZAP_INTERNAL _zap_display_entry_t* _zap_window_get_display(_zap_window_entry_t* window) {
  assert(ZAP.inited);
  assert(window);

  int x = window->rect.x;
  int y = window->rect.y;

  _ZAP_DISPLAYS_FOREACH({
    zap_recti_t rect = it->info.rect;
    if (
      rect.x <= x && x <= (rect.x + rect.width) &&
      rect.y <= y && y <= (rect.y + rect.height)
    ) {
      return it;
    }
  });

  return NULL;
}

_ZAP_INTERNAL void _zap_display_destroy(_zap_display_entry_t* display) {
  if (display == NULL) {
    return;
  }

#if defined(_ZAP_X11)
  if (display->x11_display_name) {
    free(display->x11_display_name);
    display->x11_display_name = NULL;
  }
#endif
}

_ZAP_INTERNAL _zap_display_entry_t* _zap_display_append_new(void) {
  if (ZAP.display_count >= ZAP.display_cap) {
    while (ZAP.display_count >= ZAP.display_cap) {
      ZAP.display_cap *= 2;
    }
    ZAP.displays = realloc(ZAP.displays, sizeof(_zap_display_entry_t) * ZAP.display_cap);
  }

  ZAP.displays[ZAP.display_count] = (_zap_display_entry_t) {
    .info = {
      .id = ZAP.next_display_id,
    },
  };
  _zap_display_entry_t* entry = &ZAP.displays[ZAP.display_count];
  ZAP.display_count += 1;
  return entry;
}


#if defined(_ZAP_WINDOWS)
_ZAP_INTERNAL bool _zap_windows_init(void) {
  HINSTANCE hinstance = GetModuleHandle(NULL);
  if (!hinstance) {
    return false;
  }

  WNDCLASSEX wndclass = {
    .cbSize = sizeof(WNDCLASSEX),
    .lpfnWndProc = ZapWndProc,
    .hInstance = hinstance,
    .lpszClassName = _ZAP_WINDOWS_WNDCLASS,
    .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC
    // .hIcon = LoadIcon(NULL, IDI_APPLICATION),
    // .hIconSm = LoadIcon(NULL, IDI_APPLICATION),
    // .hCursor = LoadCursor(NULL, IDC_ARROW),
  };

  if (!RegisterClassEx(&wndclass)) {
    return false;
  }

  if (!QueryPerformanceFrequency(&ZAP.qpfreq)) {
    return false;
  }

  if (!QueryPerformanceCounter(&ZAP.qpstart)) {
    return false;
  }

  ZAP.hinstance = hinstance;
  ZAP.wndclass = wndclass;

  ZAP.keycodes[0x00B] = ZAP_KEYCODE_0;
  ZAP.keycodes[0x002] = ZAP_KEYCODE_1;
  ZAP.keycodes[0x003] = ZAP_KEYCODE_2;
  ZAP.keycodes[0x004] = ZAP_KEYCODE_3;
  ZAP.keycodes[0x005] = ZAP_KEYCODE_4;
  ZAP.keycodes[0x006] = ZAP_KEYCODE_5;
  ZAP.keycodes[0x007] = ZAP_KEYCODE_6;
  ZAP.keycodes[0x008] = ZAP_KEYCODE_7;
  ZAP.keycodes[0x009] = ZAP_KEYCODE_8;
  ZAP.keycodes[0x00A] = ZAP_KEYCODE_9;
  ZAP.keycodes[0x01E] = ZAP_KEYCODE_A;
  ZAP.keycodes[0x030] = ZAP_KEYCODE_B;
  ZAP.keycodes[0x02E] = ZAP_KEYCODE_C;
  ZAP.keycodes[0x020] = ZAP_KEYCODE_D;
  ZAP.keycodes[0x012] = ZAP_KEYCODE_E;
  ZAP.keycodes[0x021] = ZAP_KEYCODE_F;
  ZAP.keycodes[0x022] = ZAP_KEYCODE_G;
  ZAP.keycodes[0x023] = ZAP_KEYCODE_H;
  ZAP.keycodes[0x017] = ZAP_KEYCODE_I;
  ZAP.keycodes[0x024] = ZAP_KEYCODE_J;
  ZAP.keycodes[0x025] = ZAP_KEYCODE_K;
  ZAP.keycodes[0x026] = ZAP_KEYCODE_L;
  ZAP.keycodes[0x032] = ZAP_KEYCODE_M;
  ZAP.keycodes[0x031] = ZAP_KEYCODE_N;
  ZAP.keycodes[0x018] = ZAP_KEYCODE_O;
  ZAP.keycodes[0x019] = ZAP_KEYCODE_P;
  ZAP.keycodes[0x010] = ZAP_KEYCODE_Q;
  ZAP.keycodes[0x013] = ZAP_KEYCODE_R;
  ZAP.keycodes[0x01F] = ZAP_KEYCODE_S;
  ZAP.keycodes[0x014] = ZAP_KEYCODE_T;
  ZAP.keycodes[0x016] = ZAP_KEYCODE_U;
  ZAP.keycodes[0x02F] = ZAP_KEYCODE_V;
  ZAP.keycodes[0x011] = ZAP_KEYCODE_W;
  ZAP.keycodes[0x02D] = ZAP_KEYCODE_X;
  ZAP.keycodes[0x015] = ZAP_KEYCODE_Y;
  ZAP.keycodes[0x02C] = ZAP_KEYCODE_Z;
  ZAP.keycodes[0x028] = ZAP_KEYCODE_APOSTROPHE;
  ZAP.keycodes[0x02B] = ZAP_KEYCODE_BACKSLASH;
  ZAP.keycodes[0x033] = ZAP_KEYCODE_COMMA;
  ZAP.keycodes[0x00D] = ZAP_KEYCODE_EQUAL;
  ZAP.keycodes[0x029] = ZAP_KEYCODE_GRAVE_ACCENT;
  ZAP.keycodes[0x01A] = ZAP_KEYCODE_LEFT_BRACKET;
  ZAP.keycodes[0x00C] = ZAP_KEYCODE_MINUS;
  ZAP.keycodes[0x034] = ZAP_KEYCODE_PERIOD;
  ZAP.keycodes[0x01B] = ZAP_KEYCODE_RIGHT_BRACKET;
  ZAP.keycodes[0x027] = ZAP_KEYCODE_SEMICOLON;
  ZAP.keycodes[0x035] = ZAP_KEYCODE_SLASH;
  ZAP.keycodes[0x056] = ZAP_KEYCODE_WORLD_2;
  ZAP.keycodes[0x00E] = ZAP_KEYCODE_BACKSPACE;
  ZAP.keycodes[0x153] = ZAP_KEYCODE_DELETE;
  ZAP.keycodes[0x14F] = ZAP_KEYCODE_END;
  ZAP.keycodes[0x01C] = ZAP_KEYCODE_ENTER;
  ZAP.keycodes[0x001] = ZAP_KEYCODE_ESCAPE;
  ZAP.keycodes[0x147] = ZAP_KEYCODE_HOME;
  ZAP.keycodes[0x152] = ZAP_KEYCODE_INSERT;
  ZAP.keycodes[0x15D] = ZAP_KEYCODE_MENU;
  ZAP.keycodes[0x151] = ZAP_KEYCODE_PAGE_DOWN;
  ZAP.keycodes[0x149] = ZAP_KEYCODE_PAGE_UP;
  ZAP.keycodes[0x045] = ZAP_KEYCODE_PAUSE;
  ZAP.keycodes[0x146] = ZAP_KEYCODE_PAUSE;
  ZAP.keycodes[0x039] = ZAP_KEYCODE_SPACE;
  ZAP.keycodes[0x00F] = ZAP_KEYCODE_TAB;
  ZAP.keycodes[0x03A] = ZAP_KEYCODE_CAPS_LOCK;
  ZAP.keycodes[0x145] = ZAP_KEYCODE_NUM_LOCK;
  ZAP.keycodes[0x046] = ZAP_KEYCODE_SCROLL_LOCK;
  ZAP.keycodes[0x03B] = ZAP_KEYCODE_F1;
  ZAP.keycodes[0x03C] = ZAP_KEYCODE_F2;
  ZAP.keycodes[0x03D] = ZAP_KEYCODE_F3;
  ZAP.keycodes[0x03E] = ZAP_KEYCODE_F4;
  ZAP.keycodes[0x03F] = ZAP_KEYCODE_F5;
  ZAP.keycodes[0x040] = ZAP_KEYCODE_F6;
  ZAP.keycodes[0x041] = ZAP_KEYCODE_F7;
  ZAP.keycodes[0x042] = ZAP_KEYCODE_F8;
  ZAP.keycodes[0x043] = ZAP_KEYCODE_F9;
  ZAP.keycodes[0x044] = ZAP_KEYCODE_F10;
  ZAP.keycodes[0x057] = ZAP_KEYCODE_F11;
  ZAP.keycodes[0x058] = ZAP_KEYCODE_F12;
  ZAP.keycodes[0x14B] = ZAP_KEYCODE_LEFT;
  ZAP.keycodes[0x14D] = ZAP_KEYCODE_RIGHT;
  ZAP.keycodes[0x148] = ZAP_KEYCODE_UP;
  ZAP.keycodes[0x052] = ZAP_KEYCODE_KP_0;
  ZAP.keycodes[0x04F] = ZAP_KEYCODE_KP_1;
  ZAP.keycodes[0x050] = ZAP_KEYCODE_KP_2;
  ZAP.keycodes[0x051] = ZAP_KEYCODE_KP_3;
  ZAP.keycodes[0x04B] = ZAP_KEYCODE_KP_4;
  ZAP.keycodes[0x04C] = ZAP_KEYCODE_KP_5;
  ZAP.keycodes[0x04D] = ZAP_KEYCODE_KP_6;
  ZAP.keycodes[0x047] = ZAP_KEYCODE_KP_7;
  ZAP.keycodes[0x048] = ZAP_KEYCODE_KP_8;
  ZAP.keycodes[0x049] = ZAP_KEYCODE_KP_9;
  ZAP.keycodes[0x04E] = ZAP_KEYCODE_KP_ADD;
  ZAP.keycodes[0x053] = ZAP_KEYCODE_KP_DECIMAL;
  ZAP.keycodes[0x135] = ZAP_KEYCODE_KP_DIVIDE;
  ZAP.keycodes[0x11C] = ZAP_KEYCODE_KP_ENTER;
  ZAP.keycodes[0x037] = ZAP_KEYCODE_KP_MULTIPLY;
  ZAP.keycodes[0x04A] = ZAP_KEYCODE_KP_SUBTRACT;
  return true;
}

_ZAP_INTERNAL zap_keymod_t _zap_windows_get_keymod(void) {
  uint32_t mod = 0;
  static int mask = 1 << 15;
  if (GetKeyState(VK_SHIFT) & mask) {
    mod |= ZAP_KEYMOD_SHIFT;
  }
  if (GetKeyState(VK_CONTROL) & mask) {
    mod |= ZAP_KEYMOD_CTRL;
  }
  if (GetKeyState(VK_MENU) & mask) {
    mod |= ZAP_KEYMOD_ALT;
  }
  if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & mask) {
    mod |= ZAP_KEYMOD_META;
  }
  return (zap_keymod_t)mod;
}

LRESULT CALLBACK ZapWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  zap_window_t window_id = GetWindowLongPtrW(hwnd, GWLP_USERDATA);

  switch (msg) {
    case WM_CLOSE: {
      zap_window_request_close(window_id);
      return 0;
    } break;

    case WM_SIZE:
    case WM_MOVE: {
      // TODO(performance) - run a timer to run this refresh method
      _zap_window_entry_t* window = _zap_window_find(window_id);
      if (window) {
        _zap_window_refresh_size(window);

        ZAP.on_event((zap_event_t) {
          .type = msg == WM_SIZE ? ZAP_EVENT_WINDOW_RESIZED : ZAP_EVENT_WINDOW_MOVED,
          .window = window_id,
        });
      }
    } break;

    case WM_MOUSEMOVE: {
      // TODO implement this
    } break;

    case WM_DROPFILES: {
      HDROP hdrop = (HDROP)wparam;
      if (hdrop) {
        ZAP.on_event((zap_event_t) {
          .type = ZAP_EVENT_FILE_DROP_STARTED,
          .window = window_id,
        });

        size_t num_files = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);

        for (size_t i = 0; i < num_files; ++i) {
          size_t path_len = DragQueryFile(hdrop, i, NULL, 0);
          char* path = malloc((path_len + 1) * sizeof(char));
          memset(path, 0, path_len + 1);

          DragQueryFile(hdrop, i, path, path_len + 1);

          ZAP.on_event((zap_event_t) {
            .type = ZAP_EVENT_FILE_DROPPED,
            .window = window_id,
            .filename = path,
          });

          free(path);
        }

        ZAP.on_event((zap_event_t) {
          .type = ZAP_EVENT_FILE_DROP_ENDED,
          .window = window_id,
        });
      }
    } break;

    case WM_KEYUP:
    case WM_KEYDOWN: {
      if (ZAP.on_event) {
        zap_keycode_t keycode = ZAP.keycodes[HIWORD(lparam) & 0x1FF];
        if (keycode) {
          bool is_repeat = msg == WM_KEYDOWN ? (lparam & 0xFF) > 0 : false;

          ZAP.on_event((zap_event_t) {
            .type = msg == WM_KEYUP ? ZAP_EVENT_KEY_UP : ZAP_EVENT_KEY_DOWN,
            .window = window_id,
            .keycode = keycode,
            .keymod = _zap_windows_get_keymod(),
            .key_repeat = is_repeat,
          });
        }
      }
    } break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

_ZAP_INTERNAL bool _zap_windows_refresh_displays(void) {
  size_t idx = 0;
  while (true) {
    DISPLAY_DEVICEW display_device = {
      .cb = sizeof(DISPLAY_DEVICEW),
    };
    if (!EnumDisplayDevicesW(NULL, idx, &display_device, EDD_GET_DEVICE_INTERFACE_NAME)) {
      break;
    }

    DEVMODEW device_mode = {
      .dmSize = sizeof(DEVMODEW),
    };
    if (!EnumDisplaySettingsExW(display_device.DeviceName, ENUM_REGISTRY_SETTINGS, &device_mode, EDS_RAWMODE)) {
      break;
    }

    _zap_windows_upsert_display(&display_device, &device_mode);

    idx += 1;
  }

  return idx > 0;
}

_ZAP_INTERNAL bool _zap_windows_upsert_display(const DISPLAY_DEVICEW *display_device, const DEVMODEW *device_mode) {
  assert(display_device);
  assert(device_mode);

  _zap_display_entry_t* entry = NULL;
  _ZAP_DISPLAYS_FOREACH({
    if (wcscmp(it->win32_device_name, display_device->DeviceName) == 0) {
      entry = it;
      break;
    }
  });

  if (!entry) {
    entry = _zap_display_append_new();
    wcscpy_s(entry->win32_device_name, 32, display_device->DeviceName);
  }

  zap_display_info_t* info = &entry->info;
  zap_recti_t* rect = &info->rect;
  rect->x = device_mode->dmPosition.x;
  rect->x = device_mode->dmPosition.y;
  rect->width = device_mode->dmPelsWidth;
  rect->height = device_mode->dmPelsHeight;
  info->refresh_rate = device_mode->dmDisplayFrequency;

  if (display_device->StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
    ZAP.primary_display = entry;
  }

  return true;
}

#elif defined(_ZAP_X11)
_ZAP_INTERNAL bool _zap_x11_init(void) {
  Display* display = XOpenDisplay(NULL);
  if (!display) {
    return false;
  }

  ZAP.xdisplay = display;
  ZAP.xroot_window = XDefaultRootWindow(display);
  ZAP.xa_wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);
  ZAP.xa_window_id = XInternAtom(display, "ZAP_WINDOW_USER_DATA", false);

  // TODO implement this -- reference: https://github.com/floooh/sokol/blob/master/sokol_app.h#L10045

  return true;
}

_ZAP_INTERNAL void _zap_x11_handle_events(void) {
  XEvent xevent = {0};
  while (XPending(ZAP.xdisplay)) {
    XNextEvent(ZAP.xdisplay, &xevent);

    switch(xevent.type) {
      case ClientMessage: {
        Atom msg_atom = (Atom)xevent.xclient.data.l[0];
        if (msg_atom == ZAP.xa_wm_delete_window) {
          zap_window_request_close(_zap_x11_get_window(xevent.xclient.window));
        }

        // TODO: handle this
        // if (xevent.xclient.data.l[0] == ZAP.xwm_delete_window) {
        //   for (size_t i = 0; i < ZAP.window_count; ++i) {
        //     ZAPWindow* window = &ZAP.windows[i];
        //     if (window->xwindow == xevent.xclient.window) {
        //       ids_to_close[num_pending_close++] = window->id;
        //     }
        //   }
        // }
      } break;
    }
  }
}

_ZAP_INTERNAL bool _zap_x11_refresh_displays(void) {
  XRRScreenResources* resources = XRRGetScreenResources(ZAP.xdisplay, ZAP.xroot_window);
  if (!resources) {
    return false;
  }

  for (int i = 0; i < resources->noutput; ++i) {
    RROutput output = resources->outputs[i];
    XRROutputInfo* output_info = XRRGetOutputInfo(ZAP.xdisplay, resources, output);
    if (!output_info) {
      XRRFreeScreenResources(resources);
      return false;
    }

    if (output_info->connection == RR_Connected && output_info->crtc) {
      XRRCrtcInfo* crtc_info = XRRGetCrtcInfo(ZAP.xdisplay, resources, output_info->crtc);
      if (!crtc_info) {
        continue;
      }

      _zap_x11_upsert_display(output_info, crtc_info);

      XRRFreeCrtcInfo(crtc_info);
    }

    XRRFreeOutputInfo(output_info);
  }

  XRRFreeScreenResources(resources);

  return true;
}

_ZAP_INTERNAL void _zap_x11_upsert_display(XRROutputInfo* output_info, XRRCrtcInfo* crtc_info) {
  assert(output_info);
  assert(crtc_info);

  _zap_display_entry_t* entry = NULL;

  _ZAP_DISPLAYS_FOREACH({
    if (strcmp(it->x11_display_name, output_info->name) == 0) {
      entry = it;
      break;
    }
  });

  if (!entry) {
    if (ZAP.display_count >= ZAP.display_cap) {
      while (ZAP.display_count >= ZAP.display_cap) {
        ZAP.display_cap *= 2;
      }
      ZAP.displays = (_zap_display_entry_t*)realloc(ZAP.displays, sizeof(_zap_display_entry_t) * ZAP.display_cap);
    }

    char* x11_display_name = (char*)malloc(sizeof(char) * output_info->nameLen);
    strncpy(x11_display_name, output_info->name, output_info->nameLen);

    ZAP.display_count += 1;
    ZAP.displays[ZAP.display_count] = (_zap_display_entry_t) {
      .id = ZAP.next_display_id,
      .x11_display_name = x11_display_name,
    };
    entry = &ZAP.displays[ZAP.display_count];
    ZAP.display_count += 1;
  }

  zap_recti_t* rect = &entry->rect;
  rect->x = crtc_info->x;
  rect->y = crtc_info->y;
  rect->width = crtc_info->width;
  rect->height = crtc_info->height;
}

_ZAP_INTERNAL zap_window_t _zap_x11_get_window(Window window) {
  Atom actual_type;
  int actual_format;
  size_t n_items;
  size_t bytes_after;
  unsigned char* data = NULL;

  if (XGetWindowProperty(
    ZAP.xdisplay,
    window,
    ZAP.xa_window_id,
    0,
    1,
    false,
    XA_INTEGER,
    &actual_type,
    &actual_format,
    &n_items,
    &bytes_after,
    &data
  ) == Success) {
    Window result = (Window)*data;
    free(data);
    return result;
  }

  return 0;
}

_ZAP_INTERNAL _zap_window_entry_t* _zap_x11_find_window_entry(Window window) {
  // TODO make this more efficient
  _ZAP_WINDOWS_FOREACH({
    if (it->xwindow == window) {
      return it;
    }
  });
  return NULL;
}

#endif // _ZAP_X11

#if defined(_ZAP_MACOS)
_ZAP_INTERNAL bool _zap_macos_init(void) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  if (!pool) {
    return false;
  }
  ZAP.nspool = pool;

  NSApplication* app = [NSApplication sharedApplication];
  if (!app) {
    return false;
  }
  ZAP.nsapp = app;

  return true;
}

_ZAP_INTERNAL void _zap_macos_destroy(void) {
  if (ZAP.nsapp) {
    [ZAP.nsapp terminate:0];
    ZAP.nsapp = NULL;
  }

  if (ZAP.nspool) {
    [ZAP.nspool release];
    ZAP.nspool = NULL;
  }
}

_ZAP_INTERNAL _zap_display_entry_t* _zap_macos_upsert_display(NSScreen* screen) {
  _zap_display_entry_t* entry = NULL;
  NSNumber* screen_number = [[screen deviceDescription] objectForKey:@"NSScreenNumber"];

  _ZAP_DISPLAYS_FOREACH({
    if ([it->nsscreen_number compare:screen_number] == 0) {
      entry = it;
      break;
    }
  });

  if (!entry) {
    entry = _zap_display_append_new();
  }

  NSRect nsrect = [screen frame];
  entry->nsscreen_number = [screen_number copy];
}

_ZAP_INTERNAL bool _zap_macos_refresh_displays(void) {
  @autoreleasepool {
    id screens = [NSScreen screens];
    for (size_t i = 0; i < [screens count]; ++i) {
      id screen = [screens objectAtIndex:i];
      _zap_display_entry_t* entry = _zap_macos_upsert_display(screen);
      if (i == 0) {
        ZAP.primary_display = entry;
      }
    }
  }
  return true;
}

#endif // _ZAP_MACOS

#undef ZAP_IMPL
#endif // ZAP_IMPL

#endif // _ZAP_H_
