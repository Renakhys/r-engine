#include "text_input.h"
#include <GLFW/glfw3.h>

static void text_input_delete_character_right(text_input *t)
{
  u32 len = t->buffer.length;
  // len must be more than 1 because it counts \0 too
  if (len > 1 && t->cursor_index < len - 1)
    darray_pop_at(&t->buffer, t->cursor_index, NULL);
}

static void text_input_delete_character_left(text_input *t)
{
  u32 len = t->buffer.length;
  // len must be more than 1 because it counts \0 too
  if (len > 1 && t->cursor_index != 0)
  {
    t->cursor_index--;
    darray_pop_at(&t->buffer, t->cursor_index, NULL);
  }
}

static void text_input_delete_word_left(text_input *t)
{
  // while removing characters array pointer should not change
  i8 *buffer = (i8 *)t->buffer.data;

  // delete all spaces
  while (t->cursor_index > 0 && buffer[t->cursor_index - 1] == ' ')
    text_input_delete_character_left(t);

  // delete word up to next space
  while (t->cursor_index > 0 && buffer[t->cursor_index - 1] != ' ')
    text_input_delete_character_left(t);

  // delete all spaces again
  while (t->cursor_index > 0 && buffer[t->cursor_index - 1] == ' ')
    text_input_delete_character_left(t);
}

static bool text_input_on_key_press(text_input *t, event_key_press *event)
{
  if (event->action == GLFW_RELEASE)
    // prevent double input on release
    return false;

  switch (event->key)
  {
  case GLFW_KEY_BACKSPACE:
    if (event->mods & GLFW_MOD_CONTROL)
      text_input_delete_word_left(t);
    else
      text_input_delete_character_left(t);
    return true;
  case GLFW_KEY_DELETE:
    text_input_delete_character_right(t);
    return true;
  case GLFW_KEY_LEFT:
    if (t->cursor_index)
      t->cursor_index--;
    return true;
  case GLFW_KEY_RIGHT:
    if (t->cursor_index < t->buffer.length - 1)
      t->cursor_index++;
    return true;
  case GLFW_KEY_END:
    t->cursor_index = t->buffer.length - 1;
    return true;
  case GLFW_KEY_HOME:
    t->cursor_index = 0;
    return true;
  default:
    break;
  }

  return false;
}

static bool text_input_on_text_input(text_input *t, event_window_text_input *event)
{
  i8 input = (i8)event->codepoint;
  darray_insert_at(&t->buffer, t->cursor_index, &input);
  t->cursor_index++;

  return true;
}

static bool on_layer_event(event_type type, void *source, void *event, void *context)
{
  UNUSED(source);

  text_input *t = (text_input *)context;
  switch (type)
  {
  case EV_KEY_PRESS:
    return text_input_on_key_press(t, (event_key_press *)event);
  case EV_TEXT_INPUT:
    return text_input_on_text_input(t, (event_window_text_input *)event);
  default:
    return false;
  }
}

void text_input_create(text_input *t)
{
  t->buffer = darray_create(256, sizeof(i8), &base_allocator);
  text_input_clear(t);
  event_handler_register(&t->ev_handler, t, on_layer_event);
}

void text_input_free(text_input *t)
{
  if (t)
  {
    darray_free(&t->buffer);
  }
}

void text_input_clear(text_input *t)
{
  t->cursor_index = 0;
  darray_clear(&t->buffer);

  i8 it = '\0';
  darray_push(&t->buffer, &it);
}
