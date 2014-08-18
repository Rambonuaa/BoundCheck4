/**************************************************************************/
/* List inplemention
   address is incremental from head to last
*/

/**************************************************************************/

#include "boundcheck.h"
#include"tree_api.h"
#include <stdio.h>

struct _node {
  __metadata_unit * value;
  struct _node * next;
};

struct _node * __list = NULL;

void __remove_element(__metadata_unit* ptr) {
  struct _node * tmp1 = __list, * tmp2;
  if(tmp1 == NULL) return;
  tmp2 = tmp1->next;
  
  /* first element */
  if(tmp1->value->base == ptr->base) {
    __list = tmp1->next;
    free(tmp1);
  }
  
  for(; tmp2 != NULL && tmp2->value->base < ptr->base;) {
    tmp1 = tmp2;
    tmp2 = tmp2->next;
  }
  if(tmp2 == NULL) return;
  if(tmp2->value->base == ptr->base) {
    tmp1->next = tmp2->next;
    free(tmp2);
  }
}

void __add_element(__metadata_unit* ptr) {
  struct _node * tmp1 = __list, * tmp2, * new;
  new = malloc(sizeof(struct _node));
  if(new == NULL) return;
  new->value = ptr;
  new->next = NULL;

  if(__list == NULL) {
    __list = new;
    return;
  }
  if(__list->value->base >= ptr->base) {
    new->next = __list;
    __list = new;
    return;
  }
  tmp2 = tmp1->next;

  for(; tmp2 != NULL && tmp2->value->base < ptr->base;) {
    tmp1 = tmp2;
    tmp2 = tmp2->next;
  }

  tmp1->next = new;
  new->next = tmp2;
}

__metadata_unit* __get_exact(void* ptr) {
  struct _node * tmp = __list;
  for(; tmp != NULL;) {
    if(tmp->value->base == (size_t)ptr)
      return tmp->value;
    if(tmp->value->base > (size_t)ptr)
      break;
    tmp = tmp->next;
  }
  return NULL;
}


__metadata_unit* __get_cont(void* ptr) {
  struct _node * tmp = __list;
  for(; tmp != NULL; tmp = tmp->next) {
    if((tmp->value->base <= (size_t)ptr)&&(tmp->value->bound>=(size_t)ptr))
      return tmp->value;
    else if(tmp->value->base > (size_t)ptr)
      break;
  }
  return NULL;
}

