#pragma once
#include "common.h"
#include "core/events.h"
#include <cglm/cglm.h>

// #define GLFW_INCLUDE_NONE
#include "gl_common.h"
#include <GLFW/glfw3.h>

// typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLi32*data);

typedef struct gl_window gl_window;

struct gl_window
{
  GLFWwindow *glfw_window;
  float w, h;
  double mouse_x, mouse_y;
  vec4s clearcolor;
  void *user_data;
  // events
  event_handler window_event_handler;
};

/// @brief create a new window
/// @param title
/// @param width
/// @param height
/// @return window pointer
gl_window *gl_window_create(const char *title, i32 width, i32 height);

/// @brief destroy window
/// @param window
void gl_window_free(gl_window *window);

/// @brief check if window is open
/// @param window
/// @return true if window is open
bool gl_window_isopen(gl_window *window);

/// @brief update window an pass to next frame
/// @param window
void gl_window_update(gl_window *window);

/// @brief force close window
/// @param window
void gl_window_close(gl_window *window);

/// @brief set window as curren in opengl context
/// @param window
void gl_window_set_current(gl_window *window);

/// @brief clear bg color of window to start rendering
/// @param window
void gl_window_clear(gl_window *window);

/// @brief check if keyboard button is pressed
/// @param window
/// @param key
/// @return true if button is pressed
bool gl_window_is_key_pressed(gl_window *window, i32 key);
