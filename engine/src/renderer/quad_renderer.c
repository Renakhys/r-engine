#include "renderer/quad_renderer.h"
#include "common.h"

typedef struct
{
  vec3s position;
  vec2s uv;
} vertex;

quad_renderer quad_renderer_create()
{
  quad_renderer r = {0};

  vertex vertices[] = {
      {{-1.0f, 1.0f, 0.0f}, {0.f, 1.f}}, // top left
      {{1.0f, 1.0f, 0.0f}, {1.f, 1.f}},  // top right
      {{1.0f, -1.0f, 0.0f}, {1.f, 0.f}}, // bottom right
      {{-1.0f, -1.0f, 0.0f}, {0.f, 0.f}} // bottom left
  };

  u32 indices[] = {
      0, 3, 2, // first triangle
      2, 1, 0  // second triangle
  };

  // must create and bind vao before anything else
  r.vao = gl_vao_create();
  gl_vao_bind(r.vao);

  r.vbo = gl_vbo_create_data(vertices, sizeof(vertices));
  r.ibo = gl_ibo_create_indices(indices, 6);

  gl_vbo_set_attribute(&r.vbo, 3, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, position));
  gl_vbo_set_attribute(&r.vbo, 2, GL_FLOAT, false, sizeof(vertex), (void *)offsetof(vertex, uv));
  return r;
}

void quad_renderer_free(quad_renderer *r)
{
  gl_vao_free(&r->vao);
  gl_vbo_free(&r->vbo);
  gl_ibo_free(&r->ibo);
}

void quad_renderer_draw(quad_renderer r)
{
  gl_vao_bind(r.vao);
  gl_ibo_bind(r.ibo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}