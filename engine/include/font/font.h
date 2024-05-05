#pragma once
#include "common.h"
#include "gl/gl_texture.h"

typedef struct _fl_character_info
{
  f32 ax; // advance: the horizontal advance e.g. the horizontal distance (in 1/64th pixels) from the origin to the origin of the next glyph. Accessed via face->glyph->advance.x.
  f32 ay;
  f32 bw; // width of glyph
  f32 bh; // height of glyph
  f32 bl; // bearingX: the horizontal bearing e.g. the horizontal position (in pixels) of the bitmap relative to the origin accessed via face->glyph->bitmap_left.
  f32 bt; // bearingY: the vertical bearing e.g. the vertical position (in pixels) of the bitmap relative to the baseline accessed via face->glyph->bitmap_top.
  f32 tx; // x offset of glyph in texture coordinates
  f32 ty; // y offset of glyph in texture coordinates
} fl_character_info;

typedef struct
{
  fl_character_info characters_info[256];
  gl_texture texture;
  f32 width;
  f32 height;
  f32 line_height;
} fl_font;

/// @brief initialize freetype
/// @return 
bool font_library_init(void);

/// @brief deinitialize freetype
void font_library_deinit(void);

/// @brief load font from disk and create font atlas
/// @param font_path path
/// @param font_size_px char height in px
/// @return font pointer
fl_font *font_create(const char *font_path, f32 font_size_px);

/// @brief free loaded font
/// @param f 
void font_free(fl_font *f);
