#include "containers/llist.h"
#include <string.h>

llist llist_create(size_t node_size, LLIST_NODE_DESTRUCTOR_FN node_destructor, const allocator *allocator)
{
  llist list;
  list.head = NULL;
  list.tail = NULL;
  list.node_size = node_size;
  list.node_destructor = node_destructor;
  list.allocator = allocator;
  return list;
}

void llist_free(llist *list)
{
  llist_node *node = list->head;
  while (node)
  {
    llist_node *next = node->next;
    if (list->node_destructor)
      // call destructor function on node data
      list->node_destructor((char *)node + sizeof(llist_node));
    list->allocator->free(node);
    node = next;
  }

  list->head = NULL;
  list->tail = NULL;
}

void llist_clear(llist *list)
{
  llist_free(list);
}

void llist_iterate_forward(llist *list, LLIST_NODE_ITERATOR_FN iterator, void *user_data)
{
  llist_iterate_forward_from(list, list->head, iterator, user_data);
}

void llist_iterate_backward(llist *list, LLIST_NODE_ITERATOR_FN iterator, void *user_data)
{
  llist_iterate_backward_from(list, list->tail, iterator, user_data);
}

void llist_iterate_forward_from(llist *list, llist_node *node, LLIST_NODE_ITERATOR_FN iterator, void *user_data)
{
  UNUSED(list);

  while (node)
  {
    bool cont = iterator(node, user_data);

    if (!cont)
      return;

    node = node->next;
  }
}

void llist_iterate_backward_from(llist *list, llist_node *node, LLIST_NODE_ITERATOR_FN iterator, void *user_data)
{
  UNUSED(list);

  while (node)
  {
    bool cont = iterator(node, user_data);

    if (!cont)
      return;

    node = node->prev;
  }
}

llist_node *llist_insert_begin(llist *list)
{
  return llist_insert_at(list, 0);
}

llist_node *llist_insert_end(llist *list)
{
  return llist_insert_at(list, -1);
}

typedef struct
{
  i32 index;
  llist_node *node;
} llist_iterator_data;

static bool counting_iterator(llist_node *node, void *user_data)
{
  llist_iterator_data *data = (llist_iterator_data *)user_data;

  if (data->index == 0)
  {
    // found node at index
    data->node = node;
    return false;
  }

  data->index--;
  return true;
}

static llist_node *insert_before(llist *list, llist_node *node)
{
  llist_node *new_node = (llist_node *)list->allocator->alloc(sizeof(llist_node) + list->node_size);
  memset(new_node, 0x00, sizeof(llist_node) + list->node_size);

  if (node == NULL)
  {
    // insert node at end of list
    if (list->tail)
    {
      list->tail->next = new_node;
      new_node->prev = list->tail;
    }

    list->tail = new_node;

    // update head if list is empty
    if (!list->head)
      list->head = new_node;
  }
  else
  {
    if (node->prev)
    {
      node->prev->next = new_node;
      new_node->prev = node->prev;
    }
    else
    {
      // no precedent node, update head of list
      list->head = new_node;
    }

    new_node->next = node;
    node->prev = new_node;
  }

  list->count++;
  return new_node;
}

static llist_node *insert_after(llist *list, llist_node *node)
{
  llist_node *new_node = (llist_node *)list->allocator->alloc(sizeof(llist_node) + list->node_size);
  memset(new_node, 0x00, sizeof(llist_node) + list->node_size);

  if (node == NULL)
  {
    // insert at begin
    if (list->head)
    {
      list->head->prev = new_node;
      new_node->next = list->head;
    }

    list->head = new_node;

    // update end if list is empty
    if (!list->tail)
      list->tail = new_node;
  }
  else
  {
    if (node->next)
    {
      node->next->prev = new_node;
      new_node->next = node->next;
    }
    else
    {
      // no next node, update tail of list
      list->tail = new_node;
    }

    new_node->prev = node;
    node->next = new_node;
  }

  return new_node;
}

llist_node *llist_insert_at(llist *list, i32 index)
{
  llist_iterator_data data;
  data.node = NULL;

  if (index >= 0)
  {
    data.index = index;
    llist_iterate_forward(list, counting_iterator, &data);
    return insert_before(list, data.node);
  }
  else
  {
    // convert index to positive and bring range to start from 0
    data.index = (index * -1) - 1;
    llist_iterate_backward(list, counting_iterator, &data);
    return insert_after(list, data.node);
  }
}

void llist_remove_node(llist *list, llist_node *node)
{
  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (list->head == node)
    list->head = node->next;
  if (list->tail == node)
    list->tail = node->prev;

  list->count--;

  if (list->node_destructor)
    // call destructor function on node data
    list->node_destructor((char *)node + sizeof(llist_node));

  list->allocator->free(node);
}

void llist_remove_index(llist *list, i32 index)
{
  llist_iterator_data data;
  data.node = NULL;

  if (index >= 0)
  {
    data.index = index;
    llist_iterate_forward(list, counting_iterator, &data);
  }
  else
  {
    // convert index to positive and bring range to start from 0
    data.index = (index * -1) - 1;
    llist_iterate_backward(list, counting_iterator, &data);
  }

  if (data.node)
    llist_remove_node(list, data.node);
}

void *llist_node_get_data(llist_node *node)
{
  return (void *)((char *)node + sizeof(llist_node));
}
