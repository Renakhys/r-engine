#pragma once
#include "common.h"
#include "application.h"
#include "gl/gl_window.h"
#include "gl/gl_framebuffer.h"
#include "core/events.h"

typedef struct application_layer application_layer;

/// @brief destructor callback for application_layer data ptr
typedef void (*LAYER_DATA_DESTRUCTOR_FN)(void *data);

/// @brief callback called every frame to render the layer
typedef bool (*LAYER_RENDER_EVENT)(application_layer *application_layer, f32 dt);

struct application_layer
{
  application *app;
  // layer_data
  void *layer_data;
  LAYER_DATA_DESTRUCTOR_FN destructor_fn;
  LAYER_RENDER_EVENT render_fn;
  // callbacks
  event_handler application_event_handler;
  event_handler layer_event_handler;

  gl_framebuffer render_target;
};

/// @brief create a new layer
/// @param app pointer to application where layer is created
/// @param layer_data custom data for layer
/// @param destructor function to be used to free layer_data
/// @return pointer to new layer
application_layer *layer_create(application *app, void *layer_data, LAYER_DATA_DESTRUCTOR_FN destructor, LAYER_RENDER_EVENT render_fn);

/// @brief free resources used by a layer, including calling layer_data destructor, automatically called by application when layer is destroyed
/// @param application_layer layer to free
void layer_free(application_layer *application_layer);

/// @brief update and render a new frame of layer
/// @param application_layer current layer
/// @param dt delta time from previuos frame
/// @return return true if next layer should be rendered, false otherwise 
bool layer_update(application_layer *application_layer, f32 dt);
