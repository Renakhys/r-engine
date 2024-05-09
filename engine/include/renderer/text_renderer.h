#pragma once
#include "common.h"
#include "batch_renderer.h"
#include "font/font.h"

typedef enum
{
  text_align_left,
  text_align_center,
  text_align_right
} text_align;

typedef struct
{
  batch_renderer *base_renderer;
  gl_shader text_shader;
} text_renderer;

typedef struct
{
  fl_font *font;
  vec4s foreground;
  f32 font_scale;
  text_align align;
  f32 min_x;
  f32 max_x;
  bool is_monospace;
} text_renderer_options;

text_renderer text_renderer_create(void);

void text_renderer_free(text_renderer *r);

void text_renderer_write_line(text_renderer *r, const text_renderer_options *opts, f32 y, const char *text, u32 text_length);

void text_renderer_progress_bar(text_renderer *r, const text_renderer_options *opts, f32 y, f32 progress, const char *text, u32 text_length);

void text_renderer_draw(text_renderer *r, const text_renderer_options *opts, f32 *x, f32 *y, const char *text);

void text_renderer_new_line(text_renderer *r, const text_renderer_options *opts, f32 *x, f32 *y);

void text_renderer_begin(text_renderer *r, u32 w, u32 h);

void text_renderer_end(text_renderer *r);
