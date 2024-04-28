#pragma once
#include <app/application.h>
#include <app/application_layer.h>

#include <gl/gl_shader.h>
#include <gl/gl_buffer.h>
#include <gl/gl_texture.h>

#include <renderer/quad_renderer.h>

typedef struct
{
  gl_shader shader;
  quad_renderer renderer;
  gl_texture texture;
} test_layer_data;

application_layer *create_test_layer(application *app);
