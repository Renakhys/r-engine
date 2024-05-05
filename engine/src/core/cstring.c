#include "core/cstring.h"
#include "core/allocator.h"
#include <string.h>

cstring cstring_create(size_t size)
{
  cstring s = {
      .data = base_allocator.alloc(size),
      .size = size,
      .lenght = 0};

  memset(s.data, 0x00, size);

  return s;
}

void cstring_free(cstring *string)
{
  base_allocator.free(string->data);
}

void cstring_resize(cstring *string, size_t new_size)
{
  if (new_size <= string->lenght)
  {
    return;
  }

  char *new_data = base_allocator.alloc(new_size);
  // copy \0
  if(string->lenght)
    memcpy(new_data, string->data, string->lenght + 1);
  base_allocator.free(string->data);
  string->data = new_data;
}

void cstring_assign(cstring *string, const char *data, size_t data_len)
{
  if (string->size < data_len + 1)
  {
    cstring_resize(string, data_len + 1);
  }
  memcpy(string->data, data, data_len);
  string->lenght = data_len;
  string->data[data_len] = '\0';
}
