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

text_renderer text_renderer_create();

void text_renderer_free(text_renderer *r);

void text_renderer_draw(text_renderer *r, fl_font *font, i32 *x, i32 *y, const char *text, vec4s fg, vec4s bg);

void text_renderer_begin(text_renderer *r, u32 w, u32 h);

void text_renderer_end(text_renderer *r);
