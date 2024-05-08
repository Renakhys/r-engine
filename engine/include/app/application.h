#pragma once
#include "common.h"
#include "gl/gl_window.h"
#include "gl/gl_shader.h"
#include "renderer/quad_renderer.h"
#include "containers/llist.h"

typedef struct application application;

struct application
{
  gl_window *window;
  quad_renderer renderer;
  gl_shader shader;
  llist layers;
};

/// @brief create a new application, with a window
/// @param app pointer where to store app data
void *application_create(application *app);

/// @brief release resources used by application, including all layers
/// @param app pointer to application
void application_free(application *app);

/// @brief check if application is still running
/// @param app pointer to application
/// @return true if window is open
bool application_is_running(application *app);

/// @brief update application frame
/// @param app pointer to application
/// @param dt delta time from previous update
void application_update(application *app, f32 dt);

/// @brief create a new layer at the end of the list of layers, Should only be called from layer_create
/// @return pointer to application layer data inside list of layers
struct application_layer *application_create_layer(application *app);
