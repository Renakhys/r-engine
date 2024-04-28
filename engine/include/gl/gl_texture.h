#pragma once
#include "common.h"

typedef struct 
{
  u32 id;
  u32 internal_format;
} gl_texture;

gl_texture gl_texture_create(u32 w, u32 h, u32 internal_format);

gl_texture gl_texture_create_fb(u32 w, u32 h, u32 internal_format, u32 data_format, u32 type);

gl_texture gl_texture_create_from_file(const char *file_path);

void gl_texture_free(gl_texture* texture);

void gl_texture_set_data(gl_texture texture, u32 x0, u32 y0, u32 w, u32 h, u32 data_format, u32 data_type, const void *data);

void gl_texture_bind(gl_texture texture, u32 slot);

u32 gl_get_active_textures();