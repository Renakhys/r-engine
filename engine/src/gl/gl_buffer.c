#include "gl/gl_buffer.h"

static u32 created_vao = 0;
static u32 deleted_vao = 0;
static u32 bound_vao = 0;

static u32 created_vbo = 0;
static u32 deleted_vbo = 0;
static u32 bound_vbo = 0;

static u32 created_ibo = 0;
static u32 deleted_ibo = 0;
static u32 bound_ibo = 0;

u32 gl_get_active_vao(void)
{
  return created_vao - deleted_vao;
}

u32 gl_get_active_vbo(void)
{
  return created_vbo - deleted_vbo;
}

u32 gl_get_active_ibo(void)
{
  return created_ibo - deleted_ibo;
}

/*
  VAO
*/

gl_vao gl_vao_create(void)
{
  gl_vao vao = {0};
  glGenVertexArrays(1, &vao.id);
  created_vao++;
  return vao;
}

void gl_vao_free(gl_vao *vao)
{
  if (vao->id)
  {
    deleted_vao++;
    glDeleteVertexArrays(1, &vao->id);
    vao->id = 0;
  }
}

void gl_vao_bind(gl_vao vao)
{
  if (vao.id == bound_vao)
    return;

  glBindVertexArray(vao.id);
  bound_vao = vao.id;
}

/*
  VBO
*/

gl_vbo gl_vbo_create(size_t size)
{
  return gl_vbo_create_data(NULL, size);
}

gl_vbo gl_vbo_create_data(const void *data, size_t size)
{
  gl_vbo vbo = {0};
  glGenBuffers(1, &vbo.id);
  created_vbo++;

  gl_vbo_bind(vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

  return vbo;
}

void gl_vbo_free(gl_vbo *vbo)
{
  if (vbo->id)
  {
    deleted_vbo++;
    glDeleteBuffers(1, &vbo->id);
    vbo->id = 0;
  }
}

void gl_vbo_set_attribute(gl_vbo *vbo, size_t size, u32 type, bool normalized, size_t stride, void *pointer)
{
  gl_vbo_bind(*vbo);
  u32 idx = vbo->attributes++;
  glVertexAttribPointer(idx, size, type, normalized ? GL_TRUE : GL_FALSE, stride, pointer);
  glEnableVertexAttribArray(idx);
}

void gl_vbo_bind(gl_vbo vbo)
{
  if (vbo.id == bound_vbo)
    return;

  glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
  bound_vbo = vbo.id;
}

void gl_vbo_load_data(gl_vbo vbo, size_t offset, const void *data, size_t size)
{
  gl_vbo_bind(vbo);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

/*
  IBO
*/

gl_ibo gl_ibo_create(size_t count)
{
  return gl_ibo_create_indices(NULL, count);
}

gl_ibo gl_ibo_create_indices(const u32 *indices, size_t count)
{
  gl_ibo ibo = {0};
  glGenBuffers(1, &ibo.id);
  created_ibo++;

  gl_ibo_bind(ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_DYNAMIC_DRAW);

  return ibo;
}

void gl_ibo_free(gl_ibo *ibo)
{
  if (ibo->id)
  {
    deleted_ibo++;
    glDeleteBuffers(1, &ibo->id);
    ibo->id = 0;
  }
}

void gl_ibo_bind(gl_ibo ibo)
{
  if (ibo.id == bound_ibo)
    return;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
  bound_ibo = ibo.id;
}

void gl_ibo_load_indices(gl_ibo ibo, size_t offset, const u32 *indices, size_t count)
{
  gl_ibo_bind(ibo);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(u32), indices);
}
