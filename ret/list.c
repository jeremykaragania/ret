#include <list.h>

/*
  list_alloc allocates a list entry specified by the data "data".
*/
struct list* list_alloc(void* data) {
  struct list* ret = malloc(sizeof(struct list));

  ret->data = data;
  ret->next = NULL;

  return ret;
}

/*
  list_free frees the list "list".
*/
void list_free(struct list** list) {
  while ((*list)->next) {
    *list = list_delete(list);
  }
}

/*
  list_insert inserts the data "data" at the beginning of the list "list".
*/
struct list* list_insert(struct list** list, void* data) {
  struct list* ret = list_alloc(data);

  ret->next = *list;
  *list = ret;

  return ret;
}

/*
  list_delete deletes the first element from the list "list".
*/
struct list* list_delete(struct list** list) {
  struct list* ret = (*list)->next;

  free(*list);
  *list = ret;

  return ret;
}
