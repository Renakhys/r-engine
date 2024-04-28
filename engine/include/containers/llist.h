#pragma once
#include "common.h"
#include "core/allocator.h"

/// @brief destructor function of user data inside list node
typedef void (*LLIST_NODE_DESTRUCTOR_FN)(void *data);

typedef struct _llist_node
{
  struct _llist_node *next;
  struct _llist_node *prev;
} llist_node;

/// @brief  iterator function for traversing list
/// @param node current node
/// @param user_data pointer to custom data used storing state while iterating
/// @return true if iteration should continue, false otherwise
typedef bool (*LLIST_NODE_ITERATOR_FN)(llist_node *node, void *user_data);

/// @brief comparator function for sorting list
/// @param a node 1
/// @param b node 2
/// @param user_data pointer to custom data used for sorting
/// @return 1 if a should be before b, -1 if b sould be after a
typedef i32(*LLIST_NODE_COMPARATOR_FN)(llist_node *a, llist_node *b, void *user_data);

typedef struct llist
{
  size_t node_size;
  i32 count;
  llist_node *head;
  llist_node *tail;

  const allocator *allocator;
  LLIST_NODE_DESTRUCTOR_FN node_destructor;
} llist;

/// @brief create a new linked list
/// @param node_size size of user data contained inside each node
/// @param node_destructor function callback used to free resources inside each node
/// @param allocator allocator used to get memory for the new nodes
/// @return list
llist llist_create(size_t node_size, LLIST_NODE_DESTRUCTOR_FN node_destructor, const allocator *allocator);

/// @brief empty list and free all nodes
/// @param list list
void llist_clear(llist *list);

/// @brief free all resources used by list
/// @param list list
void llist_free(llist *list);

/// @brief iterate each element from the begin torwards end of the list calling a iterator function
/// @param list list to iterate
/// @param iterator function callback called for each element
/// @param user_data pointer to custom data passed between iterations
void llist_iterate_forward(llist *list, LLIST_NODE_ITERATOR_FN iterator, void *user_data);

/// @brief iterate each element from the end torwards begin of the list calling a iterator function
/// @param list list to iterate
/// @param iterator function callback called for each element
/// @param user_data pointer to custom data passed between iterations
void llist_iterate_backward(llist *list, LLIST_NODE_ITERATOR_FN iterator, void *user_data);

/// @brief iterate each element from node torwards end of the list calling a iterator function
/// @param list list to iterate
/// @param node starting node
/// @param iterator function callback called for each element
/// @param user_data pointer to custom data passed between iterations
void llist_iterate_forward_from(llist *list, llist_node *node, LLIST_NODE_ITERATOR_FN iterator, void *user_data);

/// @brief iterate each element from node torwards begin of the list calling a iterator function
/// @param list list to iterate
/// @param node starting node
/// @param iterator function callback called for each element
/// @param user_data pointer to custom data passed between iterations
void llist_iterate_backward_from(llist *list, llist_node *node, LLIST_NODE_ITERATOR_FN iterator, void *user_data);

/// @brief insert element at begin of list
/// @param list list
/// @return new element node
llist_node *llist_insert_begin(llist *list);

/// @brief insert element at end of list
/// @param list list
/// @return new element node
llist_node *llist_insert_end(llist *list);

/// @brief insert element at index of list (0 -> head, -1 ->tail)
/// @param list list
/// @param index index, can be negative to insert at the end of list
/// @return new element node
llist_node *llist_insert_at(llist *list, i32 index);

/// @brief remove node from list and call its destructor function
/// @param list list
/// @param node node to remove
void llist_remove_node(llist *list, llist_node *node);

/// @brief remove node from list and call its destructor function
/// @param list list
/// @param index posizion of node to remove, can be negative to insert at the end of list
void llist_remove_index(llist *list, i32 index);

/// @brief return pointer to data of node
/// @param node node
/// @return pointer to data
void *llist_node_get_data(llist_node *node);
