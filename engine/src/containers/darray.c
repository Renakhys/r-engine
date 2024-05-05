#include "containers/darray.h"
#include "core/allocator.h"
#include <string.h>

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

darray darray_create(u64 length, u64 stride, allocator *alloc)
{
  darray a = {0};
  a.capacity = length;
  a.length = 0;
  a.stride = stride;
  a.alloc = alloc;
  u64 array_size = length * stride;
  a.data = alloc->alloc(array_size);
  return a;
}

void darray_free(darray *array)
{
  if (array)
  {
    array->capacity = 0;
    array->length = 0;
    array->alloc->free(array->data);
  }
}

void darray_reserve(darray *array, u64 new_capacity)
{
  assert(array->capacity != 0);

  if (new_capacity > array->capacity)
  {
    void *temp = array->alloc->alloc(new_capacity * array->stride);
    memcpy(temp, array->data, array->length * array->stride);
    array->alloc->free(array->data);
    array->data = temp;
  }
}

void darray_resize(darray *array, u64 new_len)
{
  if (new_len > array->capacity)
    darray_reserve(array, new_len);

  array->length = new_len;
}

void darray_push(darray *array, const void *value_ptr)
{
  darray_reserve(array, array->length + 1);
  memcpy(array->data + array->length * array->stride, value_ptr, array->stride);
  array->length++;
}

bool darray_pop(darray *array, void *dest)
{
  if (array->length == 0)
    return false;

  array->length--;
  if (dest)
  {
    memcpy(array->data + array->length * array->stride, dest, array->stride);
  }
  return true;
}

bool darray_pop_at(darray *array, u64 index, void *dest)
{
  if (array->length == 0 || index >= array->length)
    return false;

  if (dest)
  {
    memcpy(dest, array->data + index * array->stride, array->stride);
  }

  // If not on the last element, snip out the entry and copy the rest inward.
  if (index != array->length - 1)
  {
    memcpy(array->data + index * array->stride,
           array->data + (index + 1) * array->stride,
           (array->length - index + 1) * array->stride);
  }

  array->length--;
  return true;
}

void darray_insert_at(darray *array, u64 index, void *value_ptr)
{
  assert(index < array->length);
  darray_reserve(array, array->length + 1);

  // Push element(s) from index forward out by one. This should
  // even happen if inserted at the last index.
  memcpy(
      (void *)(array->data + (index + 1) * array->stride),
      (void *)(array->data + index * array->stride),
      array->stride * (array->length - index));

  // Set the value at the index
  memcpy(array->data + index * array->stride, value_ptr, array->stride);
  array->length++;
}

void darray_clear(darray *array)
{
  array->length = 0;
}
