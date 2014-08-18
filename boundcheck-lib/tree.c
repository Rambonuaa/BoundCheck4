/**************************************************************************/
/* binary sort tree(BST) inplemention
   the address in left is smaller than right
*/
/**************************************************************************/

#include "boundcheck.h"
#include"tree_api.h"
#include <stdio.h>

struct _node {
  __metadata_unit * value;
  struct _node * left, * right;
};

struct _node * __root = NULL;

void __remove_element(__metadata_unit* ptr) {
  struct _node * tmp = __root, * father = NULL;

  while(tmp != NULL) {
    if(tmp->value->base > ptr->base) { father = tmp; tmp = tmp->left; }
    else if(tmp->value->base < ptr->base) { father = tmp; tmp = tmp->right; }
    else break;
  }
  if(tmp == NULL) return;

  if(tmp->left == NULL) {
    if(__root == tmp) __root = tmp->right;
    else {
      if(father->left == tmp) father->left = tmp->right;
      else father->right = tmp->right;
    }
    free(tmp);
  }
  else if(tmp->right == NULL) {
    if(__root == tmp) __root = tmp->left;
    else {
      if(father->left == tmp) father->left = tmp->left;
      else father->right = tmp->left;
    }
    free(tmp);
  }
  else { /* two children */
    struct _node * cursor = tmp->right;
    father = tmp;
    while(cursor->left != NULL) { father = cursor; cursor = cursor->left; }
    tmp->value = cursor->value;
    if(father->left == cursor) father->left = cursor->right;
    else father->right = cursor->right;
    free(cursor);
  }
}


void __add_element(__metadata_unit* ptr) {
  enum {LEFT, RIGHT} pos;
  struct _node * new, * tmp = __root, * father = NULL;
  new = malloc(sizeof(struct _node));
  if(new == NULL) return;
  new->value = ptr;
  new->left = new->right = NULL;
  
  if(__root == NULL) {
    __root = new;
    return;
  }
  while(tmp != NULL) {
    father = tmp;
    if(tmp->value->base >= ptr->base) {
      tmp = tmp->left;
      pos = LEFT;
    }
    else {
      tmp = tmp->right;
      pos = RIGHT;
    }
  }
  if(pos == LEFT) father->left = new;
  else father->right = new;
}


__metadata_unit* __get_exact(void* ptr) {
  struct _node * tmp = __root;
  while(tmp != NULL) {
    if(tmp->value->base > (size_t)ptr) tmp = tmp->left;
    else if(tmp->value->base < (size_t)ptr) tmp = tmp->right;
    else return tmp->value;
  }
  return NULL;
}


__metadata_unit* _get_cont_rec(struct _node * node, void* ptr) {
  if(node == NULL) return NULL;
  else if(node->value->base > (size_t)ptr) return _get_cont_rec(node->left, ptr);
  else if(node->value->bound>=(size_t)ptr) return node->value;
  else return _get_cont_rec(node->right, ptr);
}


__metadata_unit* __get_cont(void* ptr) {
  return _get_cont_rec(__root, ptr);
}

