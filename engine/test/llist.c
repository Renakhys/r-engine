#include "containers/llist.h"
#include <assert.h>
#include <stdio.h>

void node_destructor(void *data)
{
  printf("destructed node with value %d\n", *(i32 *)data);
}

void insert_value_at(llist *list, i32 value, i32 index)
{
  llist_node *node = llist_insert_at(list, index);
  void *data = llist_node_get_data(node);
  *(i32 *)data = value;
}

void print_node(llist_node *node)
{
  if (node == NULL)
    printf("NULL");
  else
  {
    void *data = llist_node_get_data(node);
    printf("%d", *(i32 *)data);
  }
}

bool llist_node_printer(llist_node *node, void *user_data)
{
  void *data = llist_node_get_data(node);
  i32 *index = (i32 *)user_data;
  printf("node index %d value %d ", *index, *(i32 *)data);
  printf("prev ");
  print_node(node->prev);
  printf(" next ");
  print_node(node->next);
  printf("\n");

  (*index)++;
  return true;
}

void print_list(llist *list, const char *label)
{
  printf("\n%s begin\n", label);

  printf("head ");
  print_node(list->head);
  printf(" tail ");
  print_node(list->tail);
  printf("\n");

  i32 index = 0;
  llist_iterate_forward(list, llist_node_printer, &index);
  printf("%s end\n\n", label);
}

i32 main()
{
  llist list = llist_create(sizeof(int), node_destructor, &base_allocator);

  insert_value_at(&list, 0, -1);
  insert_value_at(&list, 1, 0);
  insert_value_at(&list, 33, -1);
  insert_value_at(&list, 5, 1);
  insert_value_at(&list, 6, -2);
  insert_value_at(&list, 7, 2);

  print_list(&list, "list filled");

  llist_remove_index(&list, 2);
  llist_remove_index(&list, -2);

  print_list(&list, "list remove");

  llist_free(&list);

  size_t leaked = get_allocated_memory();
  assert(leaked == 0);
}