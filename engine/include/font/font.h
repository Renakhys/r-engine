#pragma once
#include "common.h"
#include "gl/gl_texture.h"

typedef struct _fl_character_info
{
  float ax; // advance: the horizontal advance e.g. the horizontal distance (in 1/64th pixels) from the origin to the origin of the next glyph. Accessed via face->glyph->advance.x.
  float ay;
  float bw; // width of glyph
  float bh; // height of glyph
  float bl; // bearingX: the horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via face->glyph->bitmap_left.
  float bt; // bearingY: the vertical bearing e.g. the vertical position (in pixels) of the bitmap relative to the baseline accessed via face->glyph->bitmap_top.
  double tx; // x offset of glyph in texture coordinates
  double ty; // y offset of glyph in texture coordinates
} fl_character_info;

typedef struct
{
  fl_character_info characters_info[256];
  gl_texture texture;
  float width;
  float height;
  float line_height;
} fl_font;

bool font_library_init();

void font_library_deinit();

fl_font *font_create(const char *font_path, f32 font_size_px);

void font_free(fl_font *f);