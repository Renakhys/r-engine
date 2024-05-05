#pragma once
#include "common.h"
#include "gl/gl_common.h"

typedef struct
{
  u32 id;
} gl_vao;

gl_vao gl_vao_create(void);

void gl_vao_free(gl_vao *vao);

void gl_vao_bind(gl_vao vao);

typedef struct
{
  u32 id;
  u32 attributes;
} gl_vbo;

gl_vbo gl_vbo_create(size_t size);

gl_vbo gl_vbo_create_data(const void *data, size_t size);

void gl_vbo_set_attribute(gl_vbo *vbo, size_t size, u32 type, bool normalized, size_t stride, void *pointer);

void gl_vbo_free(gl_vbo *vbo);

void gl_vbo_bind(gl_vbo vbo);

void gl_vbo_load_data(gl_vbo vbo, size_t offset, const void *data, size_t size);

typedef struct
{
  u32 id;
} gl_ibo;

gl_ibo gl_ibo_create(size_t count);

gl_ibo gl_ibo_create_indices(const u32 *indices, size_t count);

void gl_ibo_free(gl_ibo *ibo);

void gl_ibo_bind(gl_ibo ibo);

void gl_ibo_load_indices(gl_ibo ibo, size_t offset, const u32 *indices, size_t count);

u32 gl_get_active_vao(void);

u32 gl_get_active_vbo(void);

u32 gl_get_active_ibo(void);
