#pragma once
#include "common.h"

typedef struct
{
  u32 program;
} gl_shader;

gl_shader gl_shader_create(const char *vertex_path, const char *fragment_path);

void gl_shader_free(gl_shader* shader);

void gl_shader_bind(gl_shader shader);

bool gl_shader_set_uniform_mat4(gl_shader shader, const char *uniform, mat4s value);

bool gl_shader_set_uniform_int1(gl_shader shader, const char *uniform, i32 value);

bool gl_shader_set_uniform_float1(gl_shader shader, const char *uniform, float value);

bool gl_shader_set_uniform_float2(gl_shader shader, const char *uniform, vec2s value);

bool gl_shader_set_uniform_float3(gl_shader shader, const char *uniform, vec3s value);

bool gl_shader_set_uniform_float4(gl_shader shader, const char *uniform, vec4s value);

u32 gl_get_active_shaders(void);
