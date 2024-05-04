#pragma once
#include "common.h"
#include "containers/llist.h"

typedef enum
{
  EV_WINDOW_RESIZE,
  EV_WINDOW_CLOSE,

  EV_MOUSE_MOVE,
  EV_MOUSE_BUTTON,
  EV_MOUSE_SCROLL,

  EV_KEY_PRESS,
  EV_TEXT_INPUT,

  EV_END
} event_type;

/// @brief event callback function, return true if event has been handled
typedef bool (*EVENT_CALLBACK)(event_type type, void *source, void *event, void *context);

typedef struct 
{
  void* context;
  EVENT_CALLBACK callback;
} event_handler;

typedef struct
{
  i32 button;
  i32 action;
  i32 mods;
} event_mouse_button;

typedef struct
{
  f64 pos_x;
  f64 pos_y;
} event_mouse_move;

typedef struct
{
  f64 offset_x;
  f64 offset_y;
} event_mouse_scroll;

typedef struct
{
  i32 key;
  i32 scancode;
  i32 action;
  i32 mods;
} event_key_press;

typedef struct 
{
  i32 width;
  i32 height;
} event_window_resize;

typedef struct 
{
} event_window_close;

typedef struct 
{
  u32 codepoint;
} event_window_text_input;

bool event_raise(event_handler handler, void* source, event_type type, void* event_data);

void event_handler_register(event_handler* handler, void* context, EVENT_CALLBACK callback);