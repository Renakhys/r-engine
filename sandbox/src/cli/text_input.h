#pragma once
#include <containers/darray.h>
#include <core/events.h>

typedef struct
{
  darray buffer;
  u32 cursor_index;
  event_handler ev_handler;
} text_input;

void text_input_create(text_input* t);

void text_input_free(text_input* t);

void text_input_clear(text_input *t);
