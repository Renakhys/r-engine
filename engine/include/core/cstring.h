#pragma once
#include "common.h"

typedef struct
{
  char *data;
  size_t size;
  size_t lenght;
} cstring;

cstring cstring_create(size_t size);

void cstring_free(cstring* string);

void cstring_resize(cstring* string,size_t new_size);

void cstring_assign(cstring* string,const char* data, size_t data_len);
