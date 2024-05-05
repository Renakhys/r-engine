#include <app/application.h>
#include <app/application_layer.h>

static bool on_window_resize(application_layer *layer, event_window_resize *event)
{
  gl_framebuffer_resize(&layer->render_target, event->width, event->height);
  return false;
}

static bool on_main_menu_layer_event(event_type type, void *source, void *event, void *context)
{
  application_layer *layer = (application_layer *)context;
  switch (type)
  {
  case EV_WINDOW_RESIZE:
    on_window_resize(layer, (event_window_resize *)event);
    break;

  default:
    break;
  }

  return event_raise(layer->layer_event_handler, source, layer, type, event);
}

application_layer *layer_create(application *app, void *layer_data, LAYER_DATA_DESTRUCTOR_FN destructor, LAYER_RENDER_EVENT render_fn)
{
  application_layer *l = (application_layer *)application_create_layer(app);

  l->app = app;
  l->layer_data = layer_data;
  l->destructor_fn = destructor;
  l->render_fn = render_fn;

  event_handler_register(&l->application_event_handler, on_main_menu_layer_event);
  l->layer_event_handler.callback = NULL;

  l->render_target = gl_framebuffer_create(app->window->w, app->window->h);
  gl_framebuffer_texture_attachment_add(&l->render_target, 0);
  gl_framebuffer_invalidate(&l->render_target);
  gl_framebuffer_unbind();

  return l;
}

void layer_free(application_layer *layer)
{
  gl_framebuffer_free(&layer->render_target);
  layer->destructor_fn(layer->layer_data);
}

bool layer_update(application_layer *application_layer, f32 dt)
{
  gl_framebuffer_bind(&application_layer->render_target);
  gl_framebuffer_clear(&application_layer->render_target);
  bool ret = application_layer->render_fn(application_layer, dt);
  gl_framebuffer_unbind();
  return ret;
}
