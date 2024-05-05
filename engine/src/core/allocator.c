#include "core/allocator.h"
#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define BASE_ALLOCATOR_GUARD_MAGIC_NUMBER (u32)0xDEADBEEF

// counter of how much memory has been allocated
static size_t allocated_memory = 0;
static size_t deallocated_memory = 0;

typedef struct
{
  size_t size;
} allocation_info_header;

typedef struct
{
  u32 magic_number;
} allocation_guard_tail;

static void *base_allocator_allocate(size_t size)
{
  size_t allocation_size = size;
#ifdef DEBUG
  allocation_size += sizeof(allocation_info_header);
  allocation_size += sizeof(allocation_guard_tail);
  allocated_memory += size;
#endif

  void *data = malloc(allocation_size);

#ifdef DEBUG
  if (data)
  {
    memset(data, 0xcd, allocation_size);
    // write info header and guard magic number
    allocation_info_header *header = (allocation_info_header *)data;
    allocation_guard_tail *tail = (allocation_guard_tail *)((char *)data + size + sizeof(allocation_info_header));

    header->size = size;
    tail->magic_number = BASE_ALLOCATOR_GUARD_MAGIC_NUMBER;

    // return allocated memory after header
    return (u8*)data + sizeof(allocation_info_header);
  }
#endif

  return data;
}

static void base_allocator_free(void *ptr)
{
#ifdef DEBUG
  void *data = (void *)((char *)ptr - sizeof(allocation_info_header));
  size_t size = ((allocation_info_header *)data)->size;
  size_t allocation_size = size;
  allocation_size += sizeof(allocation_info_header);
  allocation_size += sizeof(allocation_guard_tail);

  deallocated_memory += size;
  allocation_guard_tail *tail = (allocation_guard_tail *)((char *)ptr + size);
  assert(tail->magic_number == BASE_ALLOCATOR_GUARD_MAGIC_NUMBER);

  memset(data, 0xcc, allocation_size);
#else
  void *data = ptr;
#endif

  free(data);
}

allocator base_allocator = {
    .alloc = base_allocator_allocate,
    .free = base_allocator_free};

size_t get_allocated_memory(void)
{
  return allocated_memory - deallocated_memory;
}
