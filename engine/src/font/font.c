#include "font/font.h"
#include "core/allocator.h"
#include "gl/gl_common.h"
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library library = {0};

static const char *fl_get_error_message(FT_Error err)
{
#undef FTERRORS_H_
#define FT_ERRORDEF(e, v, s) \
  case e:                    \
    return s;
#define FT_ERROR_START_LIST \
  switch (err)              \
  {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H
  return "(Unknown error)";
}

bool font_library_init()
{
  FT_Error error = FT_Init_FreeType(&library);
  if (error)
  {
    log_error("ERROR: unable to initialize FreeType %s\n", fl_get_error_message(error));
    return false;
  }

  return true;
}

void font_library_deinit()
{
  FT_Done_FreeType(library);
}

static void fl_font_get_char_size(FT_Face face, u32 *w, u32 *h)
{
  FT_GlyphSlot g = face->glyph;
  FT_Int32 load_flags = FT_LOAD_RENDER | FT_LOAD_TARGET_(FT_RENDER_MODE_NORMAL);

  u32 max_char_w = 0;
  u32 max_char_h = 0;

  for (u32 i = 0; i < 256; i++)
  {
    if (FT_Load_Char(face, i, load_flags))
    {
      log_warn("Loading character %c failed!\n", i);
      continue;
    }

    u32 char_w = g->bitmap.width;
    u32 char_h = g->bitmap.rows;

    max_char_w = char_w > max_char_w ? char_w : max_char_w;
    max_char_h = char_h > max_char_h ? char_h : max_char_h;
  }

  *w = max_char_w;
  *h = max_char_h;
}

static fl_font *fl_create_atlas(FT_Face face)
{
  fl_font *atlas = base_allocator.alloc(sizeof(fl_font));

  FT_GlyphSlot g = face->glyph;

  u32 max_char_w = 0;
  u32 max_char_h = 0;

  fl_font_get_char_size(face, &max_char_w, &max_char_h);

  u32 w = max_char_w * 16;
  u32 h = max_char_h * 16;

  /* you might as well save this value as it is needed later on */
  atlas->width = w;
  atlas->height = h;
  atlas->line_height = 0;

  // create texture atlas
  u8 *texture = base_allocator.alloc(w * h);
  memset(texture, 0x00, w * h);
  atlas->texture = gl_texture_create(w, h, GL_R8);
  gl_texture_set_data(atlas->texture, 0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, texture);
  base_allocator.free(texture);

  FT_Int32 load_flags = FT_LOAD_RENDER | FT_LOAD_TARGET_(FT_RENDER_MODE_NORMAL);

  u8 *buffer = base_allocator.alloc(4 * max_char_h * max_char_w);
  for (i32 idx = 0; idx < 16; idx++)
  {
    for (i32 idy = 0; idy < 16; idy++)
    {
      u8 character = idx * 16 + idy;

      // clear buffer
      memset(buffer, 0x00, 4 * max_char_h * max_char_w);
      if (FT_Load_Char(face, character, load_flags))
        continue;

      atlas->line_height = atlas->line_height > face->size->metrics.height / 64.0 ? atlas->line_height : face->size->metrics.height / 64.0;

      atlas->characters_info[character].ax = g->advance.x >> 6;
      atlas->characters_info[character].ay = g->advance.y >> 6;
      atlas->characters_info[character].bw = g->bitmap.width;
      atlas->characters_info[character].bh = g->bitmap.rows;
      atlas->characters_info[character].bl = g->bitmap_left;
      atlas->characters_info[character].bt = g->bitmap_top;
      atlas->characters_info[character].tx = (f64)(idx) / 16;
      atlas->characters_info[character].ty = (f64)(idy) / 16;

      // render character
      u8 *ptr = &buffer[0];
      for (u32 _x = 0; _x < g->bitmap.width; _x++)
      {
        for (u32 _y = 0; _y < g->bitmap.rows; _y++)
        {
          assert(g->bitmap.width <= max_char_w);
          assert(g->bitmap.rows <= max_char_h);

          u8 val = g->bitmap.buffer[_x * g->bitmap.rows + _y];
          *ptr++ = val;
          // *ptr++ = val;
          // *ptr++ = val;
          // *ptr++ = val;
        }
      }

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      gl_texture_set_data(atlas->texture, idx * max_char_w, idy * max_char_h, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, buffer);
    }
  }
  base_allocator.free(buffer);
  return atlas;
}

fl_font *font_create(const char *font_path, f32 font_size_px)
{
  fl_font *atlas = NULL;
  FT_Face face; /* handle to face object */

  FT_Error error = FT_New_Face(library, font_path, 0, &face);
  if (error)
  {
    log_error("ERROR: unable to load font [%s] %s\n", font_path, fl_get_error_message(error));
    return atlas;
  }

  error = FT_Set_Pixel_Sizes(face, 0, font_size_px);
  // error = FT_Set_Char_Size(
  //     face,               /* handle to face object         */
  //     0,                  /* char_width in 1/64 of points  */
  //     font_size * 64,     /* char_height in 1/64 of points */
  //     device_resolution,  /* horizontal device resolution  */
  //     device_resolution); /* vertical device resolution    */

  if (error)
  {
    log_error("ERROR: unable to set font size [%s] %s\n", font_path, fl_get_error_message(error));
    FT_Done_Face(face);
    return atlas;
  }
  atlas = fl_create_atlas(face);

  FT_Done_Face(face);
  return atlas;
}

void font_free(fl_font *f)
{
  if (f)
  {
    gl_texture_free(&f->texture);
    base_allocator.free(f);
  }
}