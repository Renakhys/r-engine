#include "gl/gl_shader.h"
#include "gl/gl_common.h"
#include "core/file_utils.h"
#include <string.h>

u32 created_shaders = 0;
u32 deleted_shaders = 0;

#define SHADERS_COUNT 2

static const char *get_shader_type(GLuint type)
{
  switch (type)
  {
  case GL_VERTEX_SHADER:
    return "vertex";
  case GL_FRAGMENT_SHADER:
    return "fragment";
  default:
    return "invalid";
  }
}

static bool compile_shader_string(const char *src, GLenum type, GLuint *shader)
{
  *shader = glCreateShader(type);
  glShaderSource(*shader, 1, &src, NULL);
  glCompileShader(*shader);

  GLint compiled = 0;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled)
  {
    GLchar message[1024];
    GLsizei message_size = 0;
    glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
    log_error("could not compile %s shader\n", get_shader_type(type));
    log_error("%.*s\n", message_size, message);
    return false;
  }

  return true;
}

static bool compile_shader_file(const char *filename, GLenum type, GLuint *shader)
{
  cstring shader_source = cstring_create(256);
  Errno err = read_file(filename, &shader_source);
  if (err != 0)
  {
    log_error("failed to load `%s` shader file: %s\n", filename, strerror(err));
    if (shader_source.data)
      cstring_free(&shader_source);
    return false;
  }

  bool res = compile_shader_string(shader_source.data, type, shader);
  cstring_free(&shader_source);
  return res;
}

static GLint get_uniform_location(gl_shader shader, const char *name)
{
  GLint location = glGetUniformLocation(shader.program, name);
  // if (location == -1)
  //   log_warn("uniform %s not found on shader", name);
  return location;
}

gl_shader gl_shader_create(const char *vertex_path, const char *fragment_path)
{
  gl_shader shader = {0};
  GLuint program = 0;

  GLuint shaders[SHADERS_COUNT];
  GLuint shader_types[SHADERS_COUNT] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
  const char *shader_paths[SHADERS_COUNT] = {vertex_path, fragment_path};
  bool compilation_ok = true;

  // compilation
  for (i32 i = 0; i < SHADERS_COUNT; i++)
  {
    if (compile_shader_file(shader_paths[i], shader_types[i], &shaders[i]))
    {
      // log_trace("successfully compiled shader %s", shader_paths[i]);
    }
    else
      compilation_ok = false;
  }
  if (!compilation_ok)
    goto defer;

  // linking
  program = glCreateProgram();

  for (i32 i = 0; i < SHADERS_COUNT; i++)
  {
    glAttachShader(program, shaders[i]);
  }

  glLinkProgram(program);

  GLint linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    GLsizei message_size = 0;
    GLchar message[1024];

    glGetProgramInfoLog(program, sizeof(message), &message_size, message);
    log_error("shader link failed: %.*s\n", message_size, message);

    glDeleteProgram(program);
    program = 0;

    goto defer;
  }

  created_shaders++;

defer:
  for (i32 i = 0; i < SHADERS_COUNT; i++)
    glDeleteShader(shaders[i]);

  shader.program = program;
  return shader;
}

void gl_shader_free(gl_shader *shader)
{
  if (shader->program != 0)
  {
    deleted_shaders++;
    glDeleteProgram(shader->program);
    shader->program = 0;
  }
}

void gl_shader_bind(gl_shader shader)
{
  glUseProgram(shader.program);
}

u32 gl_get_active_shaders()
{
  return created_shaders - deleted_shaders;
}

bool gl_shader_set_uniform_mat4(gl_shader shader, const char *uniform, mat4s value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniformMatrix4fv(location, 1, GL_FALSE, &value.raw[0][0]);
  return true;
}

bool gl_shader_set_uniform_int1(gl_shader shader, const char *uniform, i32 value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniform1i(location, value);
  return true;
}

bool gl_shader_set_uniform_float1(gl_shader shader, const char *uniform, float value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniform1f(location, value);
  return true;
}

bool gl_shader_set_uniform_float2(gl_shader shader, const char *uniform, vec2s value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniform2fv(location, 1, &value.raw[0]);
  return true;
}

bool gl_shader_set_uniform_float3(gl_shader shader, const char *uniform, vec3s value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniform3fv(location, 1, &value.raw[0]);
  return true;
}

bool gl_shader_set_uniform_float4(gl_shader shader, const char *uniform, vec4s value)
{
  GLint location = get_uniform_location(shader, uniform);
  if (location == -1)
    return false;
  glUniform4fv(location, 1, &value.raw[0]);
  return true;
}