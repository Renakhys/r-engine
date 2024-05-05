#include "gl/gl_common.h"
#include "gl/gl_texture.h"
#include "core/allocator.h"
#include "core/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stbi_image.h>

u32 created_textures = 0;
u32 deleted_textures = 0;

gl_texture gl_texture_create(u32 w, u32 h, u32 internal_format)
{
  gl_texture texture = {0};
  texture.internal_format = internal_format;

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexStorage2D(GL_TEXTURE_2D, 1, texture.internal_format, w, h);

  created_textures++;

  return texture;
}

gl_texture gl_texture_create_fb(u32 w, u32 h, u32 internal_format, u32 data_format, u32 type)
{
  gl_texture texture = {0};
  texture.internal_format = internal_format;

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, data_format, type, NULL);

  created_textures++;

  return texture;
}

gl_texture gl_texture_create_from_file(const char *file_path)
{
  gl_texture texture = {0};
  stbi_set_flip_vertically_on_load(1);

  i32 width, height, channels;
  stbi_uc *data = NULL;
  {
    data = stbi_load(file_path, &width, &height, &channels, 0);
  }

  if (data)
  {
    u32 data_format = 0;
    u32 internal_format = 0;
    if (channels == 4)
    {
      data_format = GL_RGBA;
      internal_format = GL_RGBA8;
    }
    else if (channels == 3)
    {
      data_format = GL_RGB;
      internal_format = GL_RGB8;
    }
    else if (channels == 1)
    {
      data_format = GL_R8;
      internal_format = GL_R8;
    }

    texture = gl_texture_create(width, height, internal_format);
    gl_texture_set_data(texture, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }
  else
  {
    log_error("Unable to load texture %s", file_path);
  }

  return texture;
}

void gl_texture_free(gl_texture *texture)
{
  if (texture->id)
  {
    glDeleteTextures(1, &texture->id);
    deleted_textures++;
    texture->id = 0;
  }
}

void gl_texture_set_data(gl_texture texture, u32 x0, u32 y0, u32 w, u32 h, u32 data_format, u32 data_type, const void *data)
{
  if (!(texture.internal_format & data_format))
    log_error("%s", "texture format not supported!");

  // upload data to texture
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, x0, y0, w, h, data_format, data_type, data);
}

void gl_texture_bind(gl_texture texture, u32 slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

u32 gl_get_active_textures(void)
{
  return created_textures - deleted_textures;
}
