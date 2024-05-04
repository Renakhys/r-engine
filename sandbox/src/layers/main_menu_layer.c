#include "main_menu_layer.h"
#include <core/allocator.h>

static void main_menu_layer_destructor(void *layer_data)
{
  main_menu_layer_data *data = (main_menu_layer_data *)layer_data;
  base_allocator.free(data);
}

static bool main_menu_layer_render(application_layer *layer, float dt)
{
  return true;
}

static bool main_menu_on_mouse_button(application_layer *layer, event_mouse_button *event)
{
  main_menu_layer_data *data = (main_menu_layer_data *)layer->layer_data;
  printf("mouse event %s\n", data->name);
  return false;
}

static bool main_menu_on_char(application_layer *layer, event_window_text_input *event)
{
  const char* key_name = glfwGetKeyName(0,event->codepoint);
  printf("char event %c %s\n", (unsigned char)event->codepoint, key_name);
  return true;
}

static bool on_main_menu_layer_event(event_type type, void *source, void *event, void *context)
{
  application_layer *layer = (application_layer *)context;
  switch (type)
  {
  case EV_MOUSE_BUTTON:
    return main_menu_on_mouse_button(layer, (event_mouse_button *)event);
  case EV_TEXT_INPUT:
    return main_menu_on_char(layer, (event_window_text_input *)event);
  default:
    return false;
  }
}

application_layer *create_main_menu_layer(application *app, const char *name)
{
  main_menu_layer_data *data = base_allocator.alloc(sizeof(main_menu_layer_data));
  data->name = name;

  application_layer *layer = layer_create(app, data, main_menu_layer_destructor, main_menu_layer_render);
  event_handler_register(&layer->layer_event_handler, layer, on_main_menu_layer_event);

  return layer;
}