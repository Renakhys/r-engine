#pragma once
#include "common.h"

typedef void* (*ALLOCATOR_ALLOC_FN)(size_t size);
typedef void (*ALLOCATOR_FREE_FN)(void* ptr);

typedef struct 
{
  ALLOCATOR_ALLOC_FN alloc;
  ALLOCATOR_FREE_FN free;
} allocator;

extern allocator base_allocator;

size_t get_allocated_memory();