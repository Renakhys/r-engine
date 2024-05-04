#pragma once
#include "common.h"
#include "gl/gl_shader.h"
#include "gl/gl_texture.h"
#include "gl/gl_buffer.h"

typedef struct 
{
  // opengl buffers
  gl_vao vao;
  gl_vbo vbo;
  gl_ibo ibo;

  // shader
  gl_shader shader;

  // white texture
  gl_texture white_texture;

  // data buffers
  u8* vertices;
  u32* indices;
  gl_texture* textures;
  
  u32 vertices_count;
  u32 indices_count;
  u32 textures_count;

  u32 max_vertices;
  u32 max_indices;
  u32 max_textures;
  size_t vertex_size;
} batch_renderer;

batch_renderer *batch_renderer_create(size_t vertex_size, u32 vertices_count, u32 indices_count, u32 texture_count);

void batch_renderer_free(batch_renderer *r);

void batch_renderer_set_attribute(batch_renderer *r, size_t size, u32 type, bool normalized, size_t stride, void *pointer);

void batch_renderer_begin(batch_renderer *r, gl_shader shader);

void batch_renderer_end(batch_renderer *r);

u8* batch_renderer_get_quad(batch_renderer *r, gl_texture texture, u32* texture_slot);
