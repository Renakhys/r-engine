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

/// @brief create a dynamic array
/// @param length initial length
/// @param stride element size
/// @param frame_allocator memory allocator
/// @return 
darray darray_create(u64 length, u64 stride, allocator *frame_allocator);

/// @brief free dynamic array
/// @param array 
void darray_free(darray *array);

/// @brief grow capacity of dynamic array
/// @param array 
/// @param new_capacity 
void darray_reserve(darray *array, u64 new_capacity);

/// @brief change length of array data
/// @param array 
/// @param new_len 
void darray_resize(darray *array, u64 new_len);

/// @brief pop element from end of array
/// @param array 
/// @param value_ptr 
/// @return true if value is popped, false otherwise
bool darray_pop(darray *array, void *value_ptr);

/// @brief pop element from index of array
/// @param array 
/// @param index 
/// @param value_ptr 
/// @return true if value is popped, false otherwise
bool darray_pop_at(darray *array, u64 index, void *value_ptr);

/// @brief insert element at end of array
/// @param array 
/// @param value_ptr 
void darray_push(darray *array, const void *value_ptr);

/// @brief insert element at position in array
/// @param array 
/// @param index 
/// @param value_ptr 
void darray_insert_at(darray *array, u64 index, void *value_ptr);

/// @brief remove all elements from array
/// @param array 
void darray_clear(darray *array);
