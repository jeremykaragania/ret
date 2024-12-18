#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct list {
  void* data;
  struct list* next;
};

struct list* list_alloc(void* data);
void list_free(struct list** list);
struct list* list_insert(struct list** list, void* data);
struct list* list_delete(struct list** list);

#endif
