#include "cli.h"
#include <core/allocator.h>
#include <string.h>

#define CONSOLE_SIZE 1024
#define PROMPT_SIZE 256

static void cli_handle_cmd(cli_state *c, const i8 *cmd)
{
  strcat(c->console_buffer, cmd);
  strcat(c->console_buffer, "\n");
  log_trace("cli received command %s", cmd);
}

static bool cli_on_key_press(cli_state *c, event_key_press *event)
{
  if (event->action == GLFW_RELEASE)
    // prevent double input on release
    return false;

  switch (event->key)
  {
  case GLFW_KEY_ENTER:
    cli_handle_cmd(c, (i8 *)c->prompt_buffer.buffer.data);
    text_input_clear(&c->prompt_buffer);
    return true;
  default:
    break;
  }

  return false;
}

static bool on_layer_event(event_type type, void *source, void *event, void *context)
{
  UNUSED(source);

  cli_state *c = (cli_state *)context;

  bool handled = event_raise(c->prompt_buffer.ev_handler, c, &c->prompt_buffer, type, event);
  if (handled)
    return true;

  switch (type)
  {
  case EV_KEY_PRESS:
    return cli_on_key_press(c, (event_key_press *)event);
  default:
    return false;
  }
}

cli_state cli_create(const i8 *prompt)
{
  cli_state c = {0};
  c.console_buffer = base_allocator.alloc(CONSOLE_SIZE);
  c.prompt_header = prompt;
  c.prompt_buffer = text_input_create();
  memset(c.console_buffer, 0x00, CONSOLE_SIZE);

  event_handler_register(&c.ev_handler, on_layer_event);
  return c;
}

void cli_free(cli_state *c)
{
  if (c)
  {
    base_allocator.free(c->console_buffer);
    text_input_free(&c->prompt_buffer);
  }
}

void cli_console_append(cli_state *c, const i8 *fmt, ...)
{
  UNUSED(c);
  UNUSED(fmt);
}
