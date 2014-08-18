/**************************************************************************/
/* Splay tree inplemention*/
/**************************************************************************/

#include "boundcheck.h"
#include"tree_api.h"
#include <stdio.h>

struct _node {
  __metadata_unit * value;
  struct _node * left, * right, * parent;
};

struct _node * __root = NULL;

void left_rotate(struct _node * x) {
  struct _node* y = x->right;
  x->right = y->left;
  if( y->left ) y->left->parent = x;
  y->parent = x->parent;
  if( !x->parent ) __root = y;
  else if( x == x->parent->left ) x->parent->left = y;
  else x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void right_rotate(struct _node *x ) {
  struct _node *y = x->left;
  x->left = y->right;
  if( y->right ) y->right->parent = x;
  y->parent = x->parent;
  if( !x->parent ) __root = y;
  else if( x == x->parent->left ) x->parent->left = y;
  else x->parent->right = y;
  y->right = x;
  x->parent = y;
}

void splay(struct _node *x ) {
  while( x->parent ) {
    if( !x->parent->parent ) {
      if( x->parent->left == x ) right_rotate( x->parent );
      else left_rotate( x->parent );
    }
    else if( x->parent->left == x && x->parent->parent->left == x->parent ) {
      right_rotate( x->parent->parent );
      right_rotate( x->parent );
    }
    else if( x->parent->right == x && x->parent->parent->right == x->parent ) {
      left_rotate( x->parent->parent );
      left_rotate( x->parent );
    }
    else if( x->parent->left == x && x->parent->parent->right == x->parent ) {
      right_rotate( x->parent );
      left_rotate( x->parent );
    }
    else {
      left_rotate( x->parent );
      right_rotate( x->parent );
    }
  }
}
 
struct _node* subtree_minimum(struct _node *u ) {
  while( u->left ) u = u->left;
  return u;
}
 
struct _node* subtree_maximum(struct _node *u ) {
  while( u->right ) u = u->right;
  return u;
}

void replace(struct _node *u, struct _node *v ) {
  if( !u->parent ) __root = v;
  else if( u == u->parent->left ) u->parent->left = v;
  else u->parent->right = v;
  if( v ) v->parent = u->parent;
}

void __remove_element(__metadata_unit* ptr) {
  struct _node * z = __root;
  while(z != NULL) {
    if(z->value->base > ptr->base) z = z->left;
    else if(z->value->base < ptr->base) z = z->right;
    else break;
  }
  if( !z ) return;
  splay( z );

  if( !z->left ) replace( z, z->right );
  else if( !z->right ) replace( z, z->left );
  else {
    struct _node *y = subtree_minimum( z->right );
    if( y->parent != z ) {
      replace( y, y->right );
      y->right = z->right;
      y->right->parent = y;
    }
    replace( z, y );
    y->left = z->left;
    y->left->parent = y;
  }
}


void __add_element(__metadata_unit* ptr) {
  struct _node *z = __root, *p = NULL;
  while( z ) {
    p = z;
    if(z->value->base > ptr->base) z = z->left;
    else if(z->value->base < ptr->base) z = z->right;
    else return;
  }
  z = malloc(sizeof(struct _node));
  z->left = z->right = NULL;
  z->value = ptr;
  z->parent = p;
  if( !p ) __root = z;
  else if(p->value->base < z->value->base) p->right = z;
  else p->left = z;
  splay( z );
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
  else if((node->value->base <= (size_t)ptr)&&(node->value->bound >= (size_t)ptr)) return node->value;
  else return _get_cont_rec(node->right, ptr);
}


__metadata_unit* __get_cont(void* ptr) {
  return _get_cont_rec(__root, ptr);
}
