#include "renderer/batch_renderer.h"
#include "core/allocator.h"
#include "gl/gl_buffer.h"

#define BATCH_RENDERER_MAX_TEXTURES 32
#define BATCH_RENDERER_MAX_QUADS 1000
#define BATCH_RENDERER_MAX_VERTICES 4 * BATCH_RENDERER_MAX_QUADS
#define BATCH_RENDERER_MAX_INDICES 6 * BATCH_RENDERER_MAX_QUADS

static void renderer_reset_batch(batch_renderer *r);
static void renderer_flush(batch_renderer *r);
static u32 renderer_submit_texture(batch_renderer *r, gl_texture texture);

batch_renderer *batch_renderer_create(size_t vertex_size, u32 vertices_count, u32 indices_count, u32 texture_count)
{
  batch_renderer *r = base_allocator.alloc(sizeof(batch_renderer));

  r->vertices_count = 0;
  r->indices_count = 0;
  r->textures_count = 0;

  r->max_vertices = vertices_count;
  r->max_indices = indices_count;
  r->max_textures = texture_count;
  r->vertex_size = vertex_size;

  r->indices = base_allocator.alloc(indices_count * sizeof(u32));
  r->textures = base_allocator.alloc(texture_count * sizeof(gl_texture));
  r->vertices = base_allocator.alloc(vertices_count * vertex_size);

  r->white_texture = gl_texture_create(1, 1, GL_RGBA8);
  u32 data = 0xffffffff;
  gl_texture_set_data(r->white_texture, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);

  r->vao = gl_vao_create();
  gl_vao_bind(r->vao);
  r->vbo = gl_vbo_create(vertices_count * vertex_size);
  r->ibo = gl_ibo_create(indices_count);

  return r;
}

void batch_renderer_free(batch_renderer *r)
{
  if (r)
  {
    base_allocator.free(r->indices);
    base_allocator.free(r->textures);
    base_allocator.free(r->vertices);

    gl_vao_free(&r->vao);
    gl_vbo_free(&r->vbo);
    gl_ibo_free(&r->ibo);
    gl_texture_free(&r->white_texture);
    base_allocator.free(r);
  }
}

void batch_renderer_set_attribute(batch_renderer *r, size_t size, u32 type, bool normalized, size_t stride, void *pointer)
{
  gl_vbo_set_attribute(&r->vbo, size, type, normalized, stride, pointer);
}

void batch_renderer_begin(batch_renderer *r, gl_shader shader)
{
  r->shader = shader;
  gl_shader_bind(r->shader);
  renderer_reset_batch(r);
}

void batch_renderer_end(batch_renderer *r)
{
  renderer_flush(r);
}

u8 *batch_renderer_get_quad(batch_renderer *r, gl_texture texture, u32 *texture_slot)
{
  // check if should flush
  if (r->indices_count + 6 >= r->max_indices)
    renderer_flush(r);
  if (r->vertices_count + 4 >= r->max_vertices)
    renderer_flush(r);

  *texture_slot = renderer_submit_texture(r, texture);

  // set indices
  u32 i = r->vertices_count;

  r->indices[r->indices_count++] = i + 2;
  r->indices[r->indices_count++] = i + 1;
  r->indices[r->indices_count++] = i + 0;
  r->indices[r->indices_count++] = i + 0;
  r->indices[r->indices_count++] = i + 3;
  r->indices[r->indices_count++] = i + 2;

  // return pointer to vertices
  r->vertices_count += 4;

  return &r->vertices[i * r->vertex_size];
}

static void renderer_reset_batch(batch_renderer *r)
{
  r->textures_count = 0;
  r->vertices_count = 0;
  r->indices_count = 0;
}

static void renderer_flush(batch_renderer *r)
{
  gl_vao_bind(r->vao);

  // upload indices
  gl_ibo_bind(r->ibo);
  gl_ibo_load_indices(r->ibo, 0, r->indices, r->indices_count);

  // upload vertices
  gl_vbo_bind(r->vbo);
  gl_vbo_load_data(r->vbo, 0, r->vertices, r->vertices_count * r->vertex_size);

  // set shader
  gl_shader_bind(r->shader);

  // bind textures
  for (u32 t = 0; t < r->textures_count; t++)
  {
    gl_texture_bind(r->textures[t], t);

    char buff[32];
    sprintf(buff, "u_textures[%d]", t);
    gl_shader_set_uniform_int1(r->shader, buff, t);
  }
  // render
  glDrawElements(GL_TRIANGLES, r->indices_count, GL_UNSIGNED_INT, 0);
  // reset batch
  renderer_reset_batch(r);
}

static u32 renderer_submit_texture(batch_renderer *r, gl_texture texture)
{
  if (texture.id == 0)
    texture = r->white_texture;

  u32 tex;
  // check if texture is already present
  for (tex = 0; tex < r->textures_count; tex++)
    if (r->textures[tex].id == texture.id)
      return tex;

  // if texture was not found and all slots are occupied flush renderer
  if (tex > r->max_textures)
  {
    renderer_flush(r);
    tex = 0;
  }

  // if texture is new texture, add it to the buffer
  r->textures_count = tex + 1 > r->textures_count ? tex + 1 : r->textures_count;
  r->textures[tex] = texture;
  return tex;
}