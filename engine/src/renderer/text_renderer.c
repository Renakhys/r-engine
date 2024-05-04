#include "renderer/text_renderer.h"
#include "core/allocator.h"

#define VERTEX_SHADER_PATH "./assets/shaders/text.vs"
#define FRAGMENT_SHADER_PATH "./assets/shaders/text.fs"

typedef struct
{
  vec3s position;
  vec2s uv;
  vec4s foreground;
  vec4s background;
  float texture;
} text_renderer_vertex;

text_renderer text_renderer_create()
{
  text_renderer r = {0};

  r.text_shader = gl_shader_create(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
  r.base_renderer = batch_renderer_create(sizeof(text_renderer_vertex), 4 * 1000, 6 * 1000, 32);
  batch_renderer_set_attribute(r.base_renderer, 3, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, position));
  batch_renderer_set_attribute(r.base_renderer, 2, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, uv));
  batch_renderer_set_attribute(r.base_renderer, 4, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, foreground));
  batch_renderer_set_attribute(r.base_renderer, 4, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, background));
  batch_renderer_set_attribute(r.base_renderer, 1, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, texture));

  return r;
}

void text_renderer_free(text_renderer *r)
{
  if (r)
  {
    gl_shader_free(&r->text_shader);
    batch_renderer_free(r->base_renderer);
  }
}

void text_renderer_begin(text_renderer *r, u32 w, u32 h)
{
  // create vp matrix based based on viewport size
  // mat4s vp = glms_ortho(-w / 2, w / 2, -h / 2, h / 2, -1.f, 1.f);
  // mat4s vp = glms_ortho(-w / 2, w / 2, -h / 2, h / 2, -1.f, 1.f);
  mat4s vp = glms_ortho(0, w, 0, h, -1.f, 1.f);

  gl_shader_bind(r->text_shader);
  gl_shader_set_uniform_mat4(r->text_shader, "u_view_projection", vp);
  gl_shader_set_uniform_float2(r->text_shader, "u_resolution", v2(w, h));

  batch_renderer_begin(r->base_renderer, r->text_shader);
}

void text_renderer_end(text_renderer *r)
{
  batch_renderer_end(r->base_renderer);
}

void text_renderer_append(text_renderer *r, fl_font *font, const char *format, ...)
{
  // va_list args;
  // va_start(args, format);
  // // available space in renderer buffer
  // size_t available = DEBUG_TEXT_BUFFER_SIZE - (r->current_wptr - &r->buffer[0]);
  // // put data in buffer
  // int len = vsnprintf(r->current_wptr, available, format, args);
  // // increment write ptr
  // r->current_wptr += len;
  // va_end(args);
}

void text_renderer_draw(text_renderer *r, fl_font *font, i32 *_x, i32 *_y, const char *text, vec4s fg, vec4s bg)
{
  i32 base_x = *_x;
  i32 pos_x = *_x;
  i32 pos_y = *_y;
  const char *c = text;

  while (*c != '\0')
  {
    if (*c == '\n')
    {
      pos_x = base_x;
      pos_y -= font->line_height;
    }
    else
    {
      u32 tex = 0;
      text_renderer_vertex *v = (text_renderer_vertex *)batch_renderer_get_quad(r->base_renderer, font->texture, &tex);

      fl_character_info info = font->characters_info[*c];

      float min_x = pos_x + info.bl;
      float max_x = pos_x + info.bl + info.bw;
      float max_y = pos_y - font->line_height + info.bt;
      float min_y = pos_y - font->line_height + info.bt - info.bh;
      float min_uv_x = info.tx;
      float max_uv_x = info.tx + info.bw / font->width;
      float max_uv_y = info.ty;
      float min_uv_y = info.ty + info.bh / font->height;

      v[0].position = v3(min_x, max_y, 0.f);
      v[1].position = v3(max_x, max_y, 0.f);
      v[2].position = v3(max_x, min_y, 0.f);
      v[3].position = v3(min_x, min_y, 0.f);

      v[0].uv = v2(min_uv_x, max_uv_y);
      v[1].uv = v2(max_uv_x, max_uv_y);
      v[2].uv = v2(max_uv_x, min_uv_y);
      v[3].uv = v2(min_uv_x, min_uv_y);

      v[0].texture = (float)tex;
      v[1].texture = (float)tex;
      v[2].texture = (float)tex;
      v[3].texture = (float)tex;

      v[0].foreground = fg;
      v[1].foreground = fg;
      v[2].foreground = fg;
      v[3].foreground = fg;

      v[0].background = bg;
      v[1].background = bg;
      v[2].background = bg;
      v[3].background = bg;
      pos_x += info.ax;
    }
    c++;
  }

  *_x = pos_x;
  *_y = pos_y;
}
