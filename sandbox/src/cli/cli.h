#pragma once
#include <common.h>
#include <core/events.h>
#include <containers/llist.h>
#include <containers/darray.h>
#include <GLFW/glfw3.h>
#include "text_input.h"

typedef struct
{
  i8 *console_buffer;
  text_input prompt_buffer;
  const i8 *prompt_header;
  event_handler ev_handler;
} cli_state;

void cli_create(cli_state* c, const i8 *prompt);

void cli_free(cli_state *c);

void cli_console_append(cli_state *c, const i8 *fmt, ...);
