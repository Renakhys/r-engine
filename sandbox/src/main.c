#include <app/application.h>
#include <assert.h>

#include "layers/main_menu_layer.h"
#include "layers/test_layer.h"
#include "layers/cli_layer.h"

#include <gl/gl_shader.h>
#include <gl/gl_buffer.h>
#include <gl/gl_texture.h>
#include <gl/gl_framebuffer.h>

#include <font/font.h>

#if EMSCRIPTEN
#include <emscripten.h>
#endif

application *app = NULL;

void application_main_loop()
{
  application_update(app, 0.f);
}

int main()
{
  if (!font_library_init())
  {
    log_error("unable to start font engine");
    return -1;
  }

  app = application_create();

  // create_main_menu_layer(app, "layer 1");
  // create_main_menu_layer(app, "layer 2");
  // create_test_layer(app);
  create_cli_layer(app);

#if EMSCRIPTEN
  emscripten_set_main_loop(application_main_loop, 60, 1);
#else
  while (application_is_running(app))
  {
    application_main_loop();
  }
#endif

  application_free(app);
  font_library_deinit();

  size_t leaked_memory = get_allocated_memory();
  assert(leaked_memory == 0);

  size_t leaked_shaders = gl_get_active_shaders();
  assert(leaked_shaders == 0);

  size_t leaked_vao = gl_get_active_vao();
  assert(leaked_vao == 0);

  size_t leaked_vbo = gl_get_active_vbo();
  assert(leaked_vbo == 0);

  size_t leaked_ibo = gl_get_active_ibo();
  assert(leaked_ibo == 0);

  size_t leaked_textures = gl_get_active_textures();
  assert(leaked_textures == 0);

  size_t leaked_framebuffers = gl_get_active_framebuffers();
  assert(leaked_framebuffers == 0);

  return 0;
}
