#pragma once
#include "common.h"
#include "batch_renderer.h"
#include "font/font.h"

#define DEBUG_TEXT_BUFFER_SIZE 8 * 1024

typedef struct
{
  batch_renderer *base_renderer;
  gl_shader text_shader;
} text_renderer;

text_renderer text_renderer_create(void);

void text_renderer_free(text_renderer *r);

void text_renderer_draw(text_renderer *r, fl_font *font, f32 *x, f32 *y, const char *text, vec4s fg);

void text_renderer_new_line(text_renderer *r, fl_font *font, f32 base_x, f32 *x, f32 *y);

void text_renderer_begin(text_renderer *r, u32 w, u32 h);

void text_renderer_end(text_renderer *r);
