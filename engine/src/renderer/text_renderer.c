#include "renderer/text_renderer.h"
#include "core/allocator.h"
#include <string.h>

#define VERTEX_SHADER_PATH "./assets/shaders/text.vs"
#define FRAGMENT_SHADER_PATH "./assets/shaders/text.fs"

typedef struct
{
  vec3s position;
  vec2s uv;
  vec4s foreground;
  float texture;
} text_renderer_vertex;

text_renderer text_renderer_create(void)
{
  text_renderer r = {0};

  r.text_shader = gl_shader_create(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
  r.base_renderer = batch_renderer_create(sizeof(text_renderer_vertex), 4 * 1000, 6 * 1000, 32);
  batch_renderer_set_attribute(r.base_renderer, 3, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, position));
  batch_renderer_set_attribute(r.base_renderer, 2, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, uv));
  batch_renderer_set_attribute(r.base_renderer, 4, GL_FLOAT, false, sizeof(text_renderer_vertex), (void *)offsetof(text_renderer_vertex, foreground));
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
  UNUSED(r);
  UNUSED(font);
  UNUSED(format);
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

void text_renderer_new_line(text_renderer *r, const text_renderer_options *opts, f32 *x, f32 *y)
{
  UNUSED(r);

  *x = opts->min_x;
  *y -= opts->font->line_height * opts->font_scale;
}

static void text_renderer_write_char(text_renderer *r, const text_renderer_options *opts, f32 *pos_x, f32 *pos_y, char c)
{
  u32 tex = 0;
  text_renderer_vertex *v = (text_renderer_vertex *)batch_renderer_get_quad(r->base_renderer, opts->font->texture, &tex);

  fl_character_info info = opts->font->characters_info[c];

  float min_x = *pos_x + (info.bl) * opts->font_scale;
  float max_x = *pos_x + (info.bl + info.bw) * opts->font_scale;
  float max_y = *pos_y - (opts->font->line_height - info.bt) * opts->font_scale;
  float min_y = *pos_y - (opts->font->line_height - info.bt + info.bh) * opts->font_scale;

  float min_uv_x = info.tx;
  float max_uv_x = info.tx + info.bw / opts->font->width;
  float max_uv_y = info.ty;
  float min_uv_y = info.ty + info.bh / opts->font->height;

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

  v[0].foreground = opts->foreground;
  v[1].foreground = opts->foreground;
  v[2].foreground = opts->foreground;
  v[3].foreground = opts->foreground;

  if (opts->is_monospace)
    *pos_x += opts->font->characters_info[' '].ax * opts->font_scale;
  else
    *pos_x += info.ax * opts->font_scale;
}

static f32 text_renderer_get_text_size(const text_renderer_options *opts, const char *text, u32 text_length)
{
  f32 len = 0.f;
  for (u32 i = 0; i < text_length; i++)
  {
    fl_character_info info = opts->font->characters_info[text[i]];

    if (opts->is_monospace)
      len += opts->font->characters_info[' '].ax * opts->font_scale;
    else
      len += info.ax * opts->font_scale;
  }
  return len * opts->font_scale;
}

void text_renderer_write_line(text_renderer *r, const text_renderer_options *opts, f32 y, const char *text, u32 text_length)
{
  // calculate line length
  float len = text_renderer_get_text_size(opts, text, text_length);

  f32 x;
  switch (opts->align)
  {
  case text_align_left:
    x = opts->min_x;
    break;
  case text_align_center:
    x = opts->min_x + (opts->max_x - opts->min_x - len) / 2.f;
    break;
  case text_align_right:
    x = opts->max_x - len;
    break;
  }

  for (u32 i = 0; i < text_length; i++)
  {
    text_renderer_write_char(r, opts, &x, &y, text[i]);
  }
}

void text_renderer_progress_bar(text_renderer *r, const text_renderer_options *opts, f32 y, f32 progress, const char *text, u32 text_length)
{
  char tmp_buffer[16];
  sprintf(tmp_buffer, " %3.1f%% ", progress * 100.f);

  f32 fill_size = opts->font->characters_info['#'].ax * opts->font_scale;
  f32 brace_size_1 = opts->font->characters_info['['].ax * opts->font_scale;
  f32 brace_size_2 = opts->font->characters_info[']'].ax * opts->font_scale;

  // calculate len of text to print
  f32 text_size = text_renderer_get_text_size(opts, text, text_length);
  text_size += text_renderer_get_text_size(opts, tmp_buffer, strlen(tmp_buffer));
  f32 available_size = opts->max_x - opts->min_x - text_size - brace_size_1 - brace_size_2;

  f32 x = opts->min_x;

  text_renderer_draw(r, opts, &x, &y, text);
  text_renderer_write_char(r, opts, &x, &y, '[');

  f32 final_x = x + available_size;
  u32 fill = (u32)(available_size * progress / fill_size);
  for (u32 i = 0; i < fill; i++)
    text_renderer_write_char(r, opts, &x, &y, '#');

  x = final_x;

  text_renderer_write_char(r, opts, &x, &y, ']');
  text_renderer_draw(r, opts, &x, &y, tmp_buffer);
}

void text_renderer_draw(text_renderer *r, const text_renderer_options *opts, f32 *_x, f32 *_y, const char *text)
{
  f32 base_x = *_x;
  f32 pos_x = *_x;
  f32 pos_y = *_y;
  const char *c = text;

  while (*c != '\0')
  {
    if (*c == '\n')
    {
      text_renderer_new_line(r, opts, &pos_x, &pos_y);
    }
    else if (*c == '\t')
    {
      fl_character_info info = opts->font->characters_info[' '];
      pos_x += info.ax * 4 * opts->font_scale;
    }
    else
    {
      text_renderer_write_char(r, opts, &pos_x, &pos_y, *c);
    }
    c++;
  }

  *_x = pos_x;
  *_y = pos_y;
}
