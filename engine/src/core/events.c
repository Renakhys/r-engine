#include "core/events.h"
#include "containers/llist.h"

void event_handler_register(event_handler *handler, EVENT_CALLBACK callback)
{
  handler->callback = callback;
  handler->context = NULL;
}

void event_handler_register_with_context(event_handler *handler, void *context, EVENT_CALLBACK callback)
{
  handler->callback = callback;
  handler->context = context;
}

bool event_raise(event_handler handler, void *source, void *target, event_type type, void *event_data)
{
  if (!handler.callback)
    return false;

  if (handler.context)
    return handler.callback(type, source, event_data, handler.context);
  else
    return handler.callback(type, source, event_data, target);
}
