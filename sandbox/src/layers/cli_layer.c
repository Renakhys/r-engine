#include "cli_layer.h"
#include "cli/cli.h"
#include <string.h>
#include <font/font.h>
#include <renderer/text_renderer.h>

typedef struct
{
  text_renderer renderer;
  fl_font *font;
  cli_state cli;
} cli_layer_data;

static void cli_layer_destructor(void *layer_data)
{
  cli_layer_data *data = (cli_layer_data *)layer_data;

  if (data)
  {
    cli_free(&data->cli);
    font_free(data->font);
    text_renderer_free(&data->renderer);
    base_allocator.free(data);
  }
}

static bool test_layer_render(application_layer *layer, float dt)
{
  UNUSED(dt);

  cli_layer_data *data = (cli_layer_data *)layer->layer_data;

  float scale = 1.f;
  f32 viewport_w = layer->app->window->w * scale;
  f32 viewport_h = layer->app->window->h * scale;

  f32 margin = 0.003;
  f32 base_x = viewport_w * margin;
  f32 base_y = viewport_h * margin;

  f32 x = base_x;
  f32 y = viewport_h - base_y;

  // count how many lines fit console window
  float cnt_lines = (f32)viewport_h / data->font->line_height;

  vec4s heading_color = hex_to_rgba(0x149414ff);
  vec4s suggestions_color = hex_to_rgba(0xabababff);
  vec4s text_color = hex_to_rgba(0xffffffff);

  text_renderer *r = &data->renderer;
  cli_state *cli = &data->cli;
  text_renderer_begin(r, viewport_w, viewport_h);

  /*
    console content
  */
  text_renderer_draw(r, data->font, &x, &y, cli->console_buffer, text_color);

  /*
    prompt heading
  */
  if (x != base_x)
  {
    // insert a new line
    text_renderer_new_line(r, data->font, base_x, &x, &y);
  }
  text_renderer_draw(r, data->font, &x, &y, cli->prompt_header, heading_color);

  /*
    prompt content
  */
  text_renderer_draw(r, data->font, &x, &y, (i8 *)cli->prompt_buffer.buffer.data, text_color);
  text_renderer_new_line(r, data->font, base_x, &x, &y);

  /*
    suggestions
  */

  i8 buffer[1024];
  for (i32 i = 0; i < 10; i++)
  {
    sprintf(buffer, "suggestion %d\n", i + 1);
    text_renderer_draw(r, data->font, &x, &y, buffer, suggestions_color);
  }

  text_renderer_end(r);

  return true;
}

static bool on_layer_event(event_type type, void *source, void *event, void *context)
{
  UNUSED(source);

  application_layer *layer = (application_layer *)context;
  cli_layer_data *data = (cli_layer_data *)layer->layer_data;

  return event_raise(data->cli.ev_handler, layer, &data->cli, type, event);
}

application_layer *create_cli_layer(application *app)
{
  cli_layer_data *data = base_allocator.alloc(sizeof(cli_layer_data));

  // data->font = font_create("./assets/fonts/Roboto-Regular.ttf", 20);
  data->font = font_create("./assets/fonts/ShareTechMono-Regular.ttf", 22);
  data->font->line_height += 3;
  data->renderer = text_renderer_create();

  application_layer *layer = layer_create(app, data, cli_layer_destructor, test_layer_render);
  event_handler_register(&layer->layer_event_handler, on_layer_event);

  data->cli = cli_create("root@rcorp > ");

  return layer;
}
