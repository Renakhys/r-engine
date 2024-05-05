#pragma once

#ifdef __EMSCRIPTEN__
// For emscripten, instead of using glad we use its built-in support for OpenGL:
#include <GL/glew.h>
#include <emscripten/emscripten.h>
#else
#include <glad/gles2.h>
#endif

#include <core/allocator.h>

#define GL_ALLOCATOR ((allocator*)&base_allocator)
