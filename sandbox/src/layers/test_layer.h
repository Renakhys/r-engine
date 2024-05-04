#pragma once
#include <app/application.h>
#include <app/application_layer.h>

#include <gl/gl_shader.h>
#include <gl/gl_buffer.h>
#include <gl/gl_texture.h>

#include <renderer/batch_renderer.h>
#include <renderer/quad_renderer.h>
#include <renderer/text_renderer.h>
#include <font/font.h>

typedef struct
{
  gl_shader shader;
  batch_renderer *renderer;
  text_renderer textr;
  gl_texture texture;
  fl_font *font;
} test_layer_data;

application_layer *create_test_layer(application *app);
