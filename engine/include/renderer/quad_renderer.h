#pragma once
#include "common.h"
#include "gl/gl_buffer.h"
#include "gl/gl_shader.h"
#include "gl/gl_texture.h"

typedef struct
{
  gl_vao vao;
  gl_vbo vbo;
  gl_ibo ibo;
} quad_renderer;

quad_renderer quad_renderer_create();

void quad_renderer_free(quad_renderer *r);

void quad_renderer_draw(quad_renderer r);
 