#include <gl/gl_window.h>
#include <string.h>

static i32 g_context_cnt = 0;

static void error_callback(i32 error, const char *description)
{
  fprintf(stderr, "GLFW error: %d %s\n", error, description);
}

static u32 last_error_code = 0;
static void gl_debug_output(GLenum source, GLenum type, u32 id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
  UNUSED(length);
  UNUSED(userParam);

  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;

  if (id == last_error_code)
    return;
  last_error_code = id;

  const char *src = "Source: ?";
  switch (source)
  {
  case GL_DEBUG_SOURCE_API:
    src = "Source: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    src = "Source: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    src = "Source: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    src = "Source: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    src = "Source: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    src = "Source: Other";
    break;
  }

  const char *t = "Type: ?";
  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    t = "Type: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    t = "Type: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    t = "Type: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    t = "Type: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    t = "Type: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    t = "Type: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    t = "Type: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    t = "Type: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    t = "Type: Other";
    break;
  }

  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    log_error("glDebugOutput severity %s [ %s ] [ %s ] id: %d message: %s", "HIGH", src, t, id, message);
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    log_warn("glDebugOutput severity %s [ %s ] [ %s ] id: %d message: %s", "MEDIUM", src, t, id, message);
    break;
  case GL_DEBUG_SEVERITY_LOW:
    log_trace("glDebugOutput severity %s [ %s ] [ %s ] id: %d message: %s", "LOW", src, t, id, message);
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    log_info("glDebugOutput severity %s [ %s ] [ %s ] id: %d message: %s", "NOTIFICATION", src, t, id, message);
    break;
  default:
    log_trace("glDebugOutput severity %s [ %s ] [ %s ] id: %d message: %s", "?", src, t, id, message);
  }
}

// window callbacks
static void glfw_window_mouse_button_callback(GLFWwindow *glfw_window, i32 button, i32 action, i32 mods);
static void glfw_window_mouse_scroll_callback(GLFWwindow *glfw_window, double offset_x, double offset_y);
static void glfw_window_mouse_position_callback(GLFWwindow *glfw_window, double pos_x, double pos_y);
static void glfw_window_key_callback(GLFWwindow *glfw_window, i32 key, i32 scancode, i32 action, i32 mods);
static void glfw_window_char_callback(GLFWwindow *glfw_window, u32 codepoint);
static void glfw_window_framebuffer_resize_callback(GLFWwindow *glfw_window, i32 width, i32 height);
static void glfw_window_resize_callback(GLFWwindow *glfw_window, i32 width, i32 height);
static void glfw_window_close_callback(GLFWwindow *glfw_window);

static bool init_glContext(void)
{
  // context is already been created, no need to create a new one
  if (g_context_cnt > 0)
  {
    return true;
    g_context_cnt++;
  }

  if (!glfwInit())
  {
    log_critical("ERROR: %s", " unable to initialize GLFW");
    return false;
  }
  g_context_cnt++;

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  glfwSetErrorCallback(error_callback);

#ifdef __EMSCRIPTEN__
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  return true;
}

static void deinit_glContext(void)
{
  g_context_cnt--;

  if (g_context_cnt == 0)
  {
    glfwTerminate();
  }
}

static bool init_GLAD(void)
{
#ifndef __EMSCRIPTEN__
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  //{
  //   log_critical("ERROR: unable to load GLAD\n");
  //   return false;
  // }
  //  #ifndef __EMSCRIPTEN__
  //    /* Load EGL */
  //    EGLDisplay display = glfwGetEGLDisplay();
  //    i32 egl_version = gladLoaderLoadEGL(display);
  //    printf("EGL %d.%d\n", GLAD_VERSION_MAJOR(egl_version), GLAD_VERSION_MINOR(egl_version));
  //  #endif

  /* Load GLES */
  i32 gles_version = 0;
  // if (rand() % 100 < 50)
  {
    printf("\t -> using GLFW to load GLES2\n");
    gles_version = gladLoadGLES2(glfwGetProcAddress);
  }
  // else
  // {
  //   printf("\t -> using GLAD loader to load GLES2\n");
  //   gles_version = gladLoaderLoadGLES2();
  // }
  log_info("\t GLES %d.%d\n", GLAD_VERSION_MAJOR(gles_version), GLAD_VERSION_MINOR(gles_version));

  log_info("\t GL Vendor: %s", (const char *)glGetString(GL_VENDOR));
  log_info("\t GL Shading language: %s", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  log_info("\t GL Renderer: %s", (const char *)glGetString(GL_RENDERER));
  log_info("\t GL Version: %s", (const char *)glGetString(GL_VERSION));

  i32 size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
  log_info("\t GL max texture size: %d", size);

  i32 flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
  }
#endif

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return true;
}

gl_window *gl_window_create(const char *title, i32 width, i32 height)
{
  init_glContext();

  GLFWwindow *glfw_window = NULL;
  if (false)
  {
    // fullscreen
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    i32 x, y;
    glfwGetMonitorWorkarea(monitor, &x, &y, &width, &height);
    glfw_window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
  }
  else
  {
    // windowed
    glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
  }

  if (!glfw_window)
  {
    log_error("ERROR: %s", "unable to create window\n");
    return NULL;
  }

  glfwMakeContextCurrent(glfw_window);
  glfwSwapInterval(1);

  if (!init_GLAD())
  {
    glfwDestroyWindow(glfw_window);
    return NULL;
  }

  gl_window *window = GL_ALLOCATOR->alloc(sizeof(gl_window));
  memset(window, 0x00, sizeof(gl_window));
  window->glfw_window = glfw_window;
  window->w = width;
  window->h = height;
  window->clearcolor = (vec4s){0, 0, 0, 1};
  glfwSetWindowUserPointer(glfw_window, window);

  // size callbacks
  glfwSetFramebufferSizeCallback(glfw_window, glfw_window_framebuffer_resize_callback);
  glfwSetWindowSizeCallback(glfw_window, glfw_window_resize_callback);

  // input callbacks
  glfwSetKeyCallback(glfw_window, glfw_window_key_callback);
  glfwSetCharCallback(glfw_window, glfw_window_char_callback);
  glfwSetMouseButtonCallback(glfw_window, glfw_window_mouse_button_callback);
  glfwSetScrollCallback(glfw_window, glfw_window_mouse_scroll_callback);
  glfwSetCursorPosCallback(glfw_window, glfw_window_mouse_position_callback);

  // other
  glfwSetWindowCloseCallback(glfw_window, glfw_window_close_callback);
  return window;
}

bool gl_window_isopen(gl_window *window)
{
  return !glfwWindowShouldClose(window->glfw_window);
}

void gl_window_update(gl_window *window)
{
  glfwSwapBuffers(window->glfw_window);
  glfwPollEvents();
}

void gl_window_close(gl_window *window)
{
  glfwSetWindowShouldClose(window->glfw_window, GLFW_TRUE);
}

void gl_window_set_current(gl_window *window)
{
  glfwMakeContextCurrent(window->glfw_window);
}

void gl_window_clear(gl_window *window)
{
  glClearColor(window->clearcolor.r, window->clearcolor.g, window->clearcolor.b, window->clearcolor.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void gl_window_free(gl_window *window)
{
  if (window)
  {
    glfwDestroyWindow(window->glfw_window);
    GL_ALLOCATOR->free(window);
    deinit_glContext();
  }
}

bool gl_window_is_key_pressed(gl_window *window, i32 key)
{
  return glfwGetKey(window->glfw_window, key) == GLFW_PRESS;
}

#pragma region callbacks

static void glfw_window_mouse_button_callback(GLFWwindow *glfw_window, i32 button, i32 action, i32 mods)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);

  event_mouse_button event = {
      .button = button,
      .action = action,
      .mods = mods};

  event_raise(window->window_event_handler, window, EV_MOUSE_BUTTON, &event);
}

static void glfw_window_mouse_scroll_callback(GLFWwindow *glfw_window, double offset_x, double offset_y)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);

  event_mouse_scroll event = {
      .offset_x = offset_x,
      .offset_y = offset_y};

  event_raise(window->window_event_handler, window, EV_MOUSE_SCROLL, &event);
}

static void glfw_window_mouse_position_callback(GLFWwindow *glfw_window, double pos_x, double pos_y)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);
  window->mouse_x = pos_x;
  window->mouse_y = pos_y;

  event_mouse_move event = {
      .pos_x = pos_x,
      .pos_y = pos_y};

  event_raise(window->window_event_handler, window, EV_MOUSE_MOVE, &event);
}

static void glfw_window_key_callback(GLFWwindow *glfw_window, i32 key, i32 scancode, i32 action, i32 mods)
{
  // const char *key_name = glfwGetKeyName(key, 0);
  // log_trace("key pressed %s", key_name, glfw_window);

  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);

  switch (action)
  {
  case GLFW_PRESS:
    break;
  case GLFW_RELEASE:
    break;
  case GLFW_REPEAT:
    break;
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    gl_window_close(window);

  event_key_press event = {
      .key = key,
      .scancode = scancode,
      .action = action,
      .mods = mods};

  event_raise(window->window_event_handler, window, EV_KEY_PRESS, &event);
}

static void glfw_window_char_callback(GLFWwindow *glfw_window, u32 codepoint)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);

  event_window_text_input event = {
      .codepoint = codepoint};

  event_raise(window->window_event_handler, window, EV_TEXT_INPUT, &event);
}

static void glfw_window_framebuffer_resize_callback(GLFWwindow *glfw_window, i32 width, i32 height)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);
  window->w = width;
  window->h = height;
  glViewport(0, 0, width, height);

  event_window_resize event = {
      .width = width,
      .height = height};

  event_raise(window->window_event_handler, window, EV_WINDOW_RESIZE, &event);
}

static void glfw_window_resize_callback(GLFWwindow *glfw_window, i32 width, i32 height)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);
  window->w = width;
  window->h = height;
  glViewport(0, 0, width, height);

  event_window_resize event = {
      .width = width,
      .height = height};

  event_raise(window->window_event_handler, window, EV_WINDOW_RESIZE, &event);
}

static void glfw_window_close_callback(GLFWwindow *glfw_window)
{
  gl_window *window = (gl_window *)glfwGetWindowUserPointer(glfw_window);

  event_window_close event;

  event_raise(window->window_event_handler, window, EV_WINDOW_CLOSE, &event);
}

#pragma endregion
