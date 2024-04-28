#include "test_layer.h"
#include <core/allocator.h>

static void test_layer_destructor(void *layer_data)
{
  test_layer_data *data = (test_layer_data *)layer_data;

  gl_shader_free(&data->shader); 
  quad_renderer_free(&data->renderer);
  gl_texture_free(&data->texture);

  base_allocator.free(data);
}

static bool test_layer_render(application_layer *layer, float dt)
{
  test_layer_data *data = (test_layer_data *)layer->layer_data;
  gl_shader_bind(data->shader);
  gl_texture_bind(data->texture, 0);
  gl_shader_set_uniform_int1(data->shader, "ourTexture", 0);

  quad_renderer_draw(data->renderer);

  return true;
}

application_layer *create_test_layer(application *app)
{
  test_layer_data *data = base_allocator.alloc(sizeof(test_layer_data));

  data->shader = gl_shader_create("./assets/shaders/quad.vs", "./assets/shaders/quad.fs");
  data->renderer = quad_renderer_create();
  data->texture = gl_texture_create_from_file("assets/textures/whiteface.png");

  application_layer *layer = layer_create(app, data, test_layer_destructor, test_layer_render);

  return layer;
}