#pragma once
#include "common.h"
#include "core/allocator.h"

typedef struct 
{
  u64 capacity;
  u64 length;
  u64 stride;
  allocator *alloc;
  u8* data;
} darray;

darray darray_create(u64 length, u64 stride, allocator *frame_allocator);

void darray_free(darray *array);

void darray_reserve(darray *array, u64 new_capacity);

void darray_resize(darray *array, u64 new_len);

bool darray_pop(darray *array, void *value_ptr);

bool darray_pop_at(darray *array, u64 index, void *value_ptr);

void darray_push(darray *array, const void *value_ptr);

void darray_insert_at(darray *array, u64 index, void *value_ptr);

void darray_clear(darray *array);
