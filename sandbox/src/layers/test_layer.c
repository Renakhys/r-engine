#include "test_layer.h"
#include <core/allocator.h>

typedef struct
{
  vec3s position;
  vec2s uv;
  float texture;
} test_layer_vertex;

static bool test_layer_render(application_layer *layer, float dt);

static void test_layer_destructor(void *layer_data)
{
  test_layer_data *data = (test_layer_data *)layer_data;

  gl_shader_free(&data->shader);
  gl_texture_free(&data->texture);
  batch_renderer_free(data->renderer);
  text_renderer_free(&data->textr);
  font_free(data->font);

  base_allocator.free(data);
}

application_layer *create_test_layer(application *app)
{
  test_layer_data *data = base_allocator.alloc(sizeof(test_layer_data));

  data->shader = gl_shader_create("./assets/shaders/batch.vs", "./assets/shaders/batch.fs");
  data->texture = gl_texture_create_from_file("assets/textures/whiteface.png");
  data->font = font_create("./assets/fonts/Good Old DOS.ttf", 22);

  data->textr = text_renderer_create();
  data->renderer = batch_renderer_create(sizeof(test_layer_vertex), 4 * 1000, 6 * 1000, 32);
  batch_renderer_set_attribute(data->renderer, 3, GL_FLOAT, false, sizeof(test_layer_vertex), (void *)offsetof(test_layer_vertex, position));
  batch_renderer_set_attribute(data->renderer, 2, GL_FLOAT, false, sizeof(test_layer_vertex), (void *)offsetof(test_layer_vertex, uv));
  batch_renderer_set_attribute(data->renderer, 1, GL_FLOAT, false, sizeof(test_layer_vertex), (void *)offsetof(test_layer_vertex, texture));

  application_layer *layer = layer_create(app, data, test_layer_destructor, test_layer_render);

  return layer;
}

static bool test_layer_render(application_layer *layer, float dt)
{
  test_layer_data *data = (test_layer_data *)layer->layer_data;
  gl_shader_bind(data->shader);
  gl_texture_bind(data->texture, 0);
  // gl_texture_bind(data->font->texture, 0);
  gl_shader_set_uniform_int1(data->shader, "ourTexture", 0);
  // quad_renderer_draw(data->qrend);

  batch_renderer *r = data->renderer;
  batch_renderer_begin(r, data->shader);

  for (i32 x = 0; x < 0; x++)
  {
    for (i32 y = 0; y < 1; y++)
    {
      float px = (float)x * 2.f - 1.f;
      float py = (float)y * 2.f - 1.f;
      float d = 2.f;

      gl_texture texture = (x + y) % 2 ? data->texture : data->font->texture;

      u32 tex = 0;
      test_layer_vertex *v = (test_layer_vertex *)batch_renderer_get_quad(r, texture, &tex);

      v[0].position = v3(px, py + d, 0.f);
      v[1].position = v3(px + d, py + d, 0.f);
      v[2].position = v3(px + d, py, 0.f);
      v[3].position = v3(px, py, 0.f);

      v[0].uv = v2(0, 1);
      v[1].uv = v2(1, 1);
      v[2].uv = v2(1, 0);
      v[3].uv = v2(0, 0);

      v[0].texture = (float)tex;
      v[1].texture = (float)tex;
      v[2].texture = (float)tex;
      v[3].texture = (float)tex;
    }
  }

  batch_renderer_end(r);

  float scale = 1;
  f32 x = 0;
  f32 y = layer->app->window->h / scale;
  text_renderer_begin(&data->textr, layer->app->window->w / scale, layer->app->window->h / scale);
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 1\n", v4(1, 0, 0, 1));
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 2\n", v4(0, 1, 0, 1));
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 3\n", v4(0, 0, 1, 1));
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 4\n", v4(1, 1, 0, 1));
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 5\n", v4(0, 1, 1, 1));
  text_renderer_draw(&data->textr, data->font, &x, &y, "test string 6\n", v4(1, 0, 1, 1));
  text_renderer_end(&data->textr);

  return true;
}