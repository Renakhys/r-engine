#include <app/application.h>
#include <app/application_layer.h>
#include <core/allocator.h>

typedef struct
{
  event_type type;
  void *source;
  void *event;
  void *context;
} application_event_data;

static bool application_layer_on_event_iterator(llist_node *node, void *user_data)
{
  application_layer *layer = (application_layer *)llist_node_get_data(node);
  application_event_data *data = (application_event_data *)user_data;

  bool handled = event_raise(layer->application_event_handler, data->source, data->type, data->event);

  if (handled)
    return false; // stop iterating
  return true;
}

static bool application_on_event(event_type type, void *source, void *event, void *context)
{
  application *app = (application *)context;

  switch (type)
  {
  default:
    break;
  }

  application_event_data data = {
      .type = type,
      .source = source,
      .event = event,
      .context = context};

  // call event on all layers
  llist_iterate_backward(&app->layers, application_layer_on_event_iterator, &data);

  return true;
}

static void application_layer_destructor(void *data)
{
  application_layer *l = (application_layer *)data;
  layer_free(l);
}

application *application_create()
{
  application *app = base_allocator.alloc(sizeof(application));

  app->window = gl_window_create("application", 800, 600);
  app->window->user_data = app;
  app->window->clearcolor = v4(1.f, 0.4f, 0.7f, 1.0f);

  // window event handler
  event_handler_register(&app->window->window_event_handler, app, application_on_event);

  app->shader = gl_shader_create("./assets/shaders/quad.vs", "./assets/shaders/quad.fs");
  app->renderer = quad_renderer_create();

  app->layers = llist_create(sizeof(application_layer), application_layer_destructor, &base_allocator);

  return app;
}

void application_free(application *app)
{
  gl_shader_free(&app->shader);
  quad_renderer_free(&app->renderer);
  llist_free(&app->layers);
  gl_window_free(app->window);
  base_allocator.free(app);
}

bool application_is_running(application *app)
{
  return gl_window_isopen(app->window);
}

typedef struct
{
  float dt;
  application *app;
} layer_update_iterator_data;

static bool application_layer_render(llist_node *node, void *user_data)
{
  application_layer *layer = (application_layer *)llist_node_get_data(node);
  layer_update_iterator_data *data = (layer_update_iterator_data *)user_data;

  // update layer
  bool ret = layer_update(layer, data->dt);

  gl_shader_bind(data->app->shader);
  gl_texture_bind(layer->render_target.attachments[0].texture, 0);
  gl_shader_set_uniform_int1(data->app->shader, "ourTexture", 0);

  quad_renderer_draw(data->app->renderer);

  // if (layer->render_fn)
  //   return layer->render_fn(layer, 0);

  return true;
}

void application_update(application *app, float dt)
{
  gl_window_set_current(app->window);
  gl_window_clear(app->window);

  // render all layers
  layer_update_iterator_data data = {
      .dt = dt,
      .app = app};

  llist_iterate_forward(&app->layers, application_layer_render, &data);

  gl_window_update(app->window);
}

struct application_layer *application_create_layer(application *app)
{
  llist_node *node = llist_insert_end(&app->layers);
  return (struct application_layer *)llist_node_get_data(node);
}