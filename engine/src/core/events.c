#include "core/events.h"
#include "containers/llist.h"

void event_handler_register(event_handler* handler, void* context, EVENT_CALLBACK callback)
{
handler->context = context;
handler->callback =callback;
}

bool event_raise(event_handler handler, void* source, event_type type, void* event_data)
{
  if(!handler.callback) return false;
  return handler.callback(type, source, event_data, handler.context);
}