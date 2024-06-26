#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "core/log.h"
// #include "core/file_utils.h"
// #include "core/cstring.h"

#include <cglm/struct.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef uint64_t u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

#define max(a, b) ((a) > (b) ? (a) : (b))

#define min(a, b) ((a) < (b) ? (a) : (b))

#define v2(x, y) ((vec2s){(float)x, (float)y})
#define v3(x, y, z) ((vec3s){(float)x, (float)y, (float)z})
#define v4(x, y, z, w) ((vec4s){(float)x, (float)y, (float)z, (float)w})

#define v2_3(v2, z) ((vec3s){v2.x, v2.y, (float)z})
#define v2_4(v2, z, w) ((vec4s){v2.x, v2.y, (float)z, (float)w})
#define v3_4(v3, w) ((vec4s){v3.x, v3.y, v3.z, (float)w})

#define v4_3(v4) ((vec3s){v4.x, v4.y, v4.z})
#define v4_2(v4) ((vec2s){v4.x, v4.y})
#define v3_2(v3) ((vec2s){v3.x, v3.y})

#define UNUSED(x) (void)(x)

static vec4s hex_to_rgba(int hex)
{
  vec4s color;
  color.r = ((hex >> 24) & 0xff) / 255.0;
  color.g = ((hex >> 16) & 0xff) / 255.0;
  color.b = ((hex >> 8) & 0xff) / 255.0;
  color.a = ((hex) & 0xff) / 255.0;

  return color;
}

static vec3s hex_to_rgb(int hex)
{
  vec3s color;
  color.r = ((hex >> 16) & 0xff) / 255.0;
  color.g = ((hex >> 8) & 0xff) / 255.0;
  color.b = ((hex) & 0xff) / 255.0;

  return color;
}
