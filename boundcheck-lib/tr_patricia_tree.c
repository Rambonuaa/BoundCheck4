/**************************************************************************/
/* Patricia tries inplemention
*/
/**************************************************************************/

#include<assert.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

#include "boundcheck.h"
#include"tree_api.h"
#include <stdio.h>

#define WORDBITS 32

const size_t Tmasks[] = {
0x0,0x80000000,0xc0000000,0xe0000000,0xf0000000,0xf8000000,0xfc000000,
0xfe000000,0xff000000,0xff800000,0xffc00000,0xffe00000,0xfff00000,0xfff80000,
0xfffc0000,0xfffe0000,0xffff0000,0xffff8000,0xffffc000,0xffffe000,0xfffff000,
0xfffff800,0xfffffc00,0xfffffe00,0xffffff00,0xffffff80,0xffffffc0,0xffffffe0,
0xfffffff0,0xfffffff8,0xfffffffc,0xfffffffe,0xffffffff};

const int Teq[] = 
  { 0,-1,3,-3,6,-5,7,-7,12,-9,11,-11,14,-13,15,-15,24,-17,19,-19,22,
    -21,23,-23,28,-25,27,-27,30,-29,31,32,-32 };

const int Tneq[] = 
  { 0,0,1,-2,2,-4,5,-6,4,-8,9,-10,10,-12,13,-14,8,-16,17,-18,18,-20,21,-22,20,
    -24,25,-26,26,-28,29,-30,-31 };

struct bittree {
  _Bool is_leaf;
  size_t addr,  mask;
  struct bittree * left, * right, * father;
  __metadata_unit * leaf;
} * __root = NULL;

size_t mask(size_t a, size_t b) {
  size_t nxor = ~(a ^ b), ret;
  int i = WORDBITS/2; 
  while(i > 0) {
    if (nxor >= Tmasks[i])
      i = Teq[i];
    else
      i = Tneq[i];
  }
  ret = Tmasks[-i];
  assert ((a & ret) == (b & ret));
  return ret;
}

struct bittree * __get_leaf_from_block (__metadata_unit * ptr) {
  struct bittree * curr = __root;
  assert(__root != NULL);
  assert(ptr != NULL);

  while(!curr->is_leaf) {
    assert((curr->addr & curr->mask) == (ptr->base & curr->mask));
    
    assert(curr->left != NULL && curr->right != NULL);
    if((curr->right->addr & curr->right->mask)
       == (ptr->base & curr->right->mask))
      curr = curr->right;
    else if((curr->left->addr & curr->left->mask)
	    == (ptr->base & curr->left->mask))
      curr = curr->left;
    else
      assert(0);
  }
  assert(curr->is_leaf);
  assert(curr->leaf == ptr);
  return curr;
}

void __remove_element (__metadata_unit * ptr) {
  struct bittree * leaf_to_delete = __get_leaf_from_block (ptr);
  assert(leaf_to_delete->leaf == ptr);
  
  if(leaf_to_delete->father == NULL)
    __root = NULL;
  else {
    struct bittree * brother, * father;
    father = leaf_to_delete->father;
    brother = (leaf_to_delete == father->left) ? father->right : father->left;
    assert(brother != NULL);
    father->is_leaf = brother->is_leaf;
    father->addr = brother->addr;
    father->mask = brother->mask;
    father->left = brother->left;
    father->right = brother->right;
    father->leaf = brother->leaf;
    if(!brother->is_leaf) {
      brother->left->father = father;
      brother->right->father = father;
    }
    free(brother);
    if(father->father != NULL) {
      father->father->mask = mask(father->father->left->addr
				  & father->father->left->mask,
				  father->father->right->addr
				  & father->father->right->mask);
    }
  }
  free(leaf_to_delete);
}

struct bittree * __most_similar_node (__metadata_unit * ptr) {
  struct bittree * curr = __root;
  size_t left_prefix, right_prefix;
  assert(ptr != NULL);
  assert(__root != NULL);

  while(1) {
    if(curr->is_leaf)
      return curr;
    assert(curr->left != NULL && curr->right != NULL);
    left_prefix = mask(curr->left->addr & curr->left->mask, ptr->base);
    right_prefix = mask(curr->right->addr & curr->right->mask, ptr->base);
    if(left_prefix > right_prefix)
      curr = curr->left;
    else if(right_prefix > left_prefix)
      curr = curr->right;
    else
      return curr;
  }
}

void __add_element (__metadata_unit * ptr) {
  struct bittree * new_leaf;
  assert(ptr != NULL);
  
  new_leaf = malloc(sizeof(struct bittree));
  assert(new_leaf != NULL);
  new_leaf->is_leaf = true;
  new_leaf->addr = ptr->base;
  new_leaf->mask = Tmasks[WORDBITS]; /* ~0ul */
  new_leaf->left = NULL;
  new_leaf->right = NULL;
  new_leaf->father = NULL;
  new_leaf->leaf = ptr;

  if(__root == NULL)
    __root = new_leaf;
  else {
    struct bittree * brother = __most_similar_node (ptr), * father, * aux;

    printf("brother %p mask %p   new_leaf %p  mask %p\n",(void *)brother->addr,(void *)brother->mask,(void *)new_leaf->addr,(void *)new_leaf->mask);
    
    assert(brother != NULL);
    father = malloc(sizeof(struct bittree));
    assert(father != NULL);
    father->is_leaf = false;
    father->addr = brother->addr & new_leaf->addr;
    /*father->mask = mask(brother->addr & brother->mask, ptr->ptr);*/
    father->leaf = NULL;
    
    printf("father %p\n",(void *)father->addr);

    if(new_leaf->addr <= brother->addr) {
      father->left = new_leaf;
      father->right = brother;
    } else {
      father->left = brother;
      father->right = new_leaf;
    }
    new_leaf->father = father;
    
    if(brother == __root) {
      father->father = NULL;
      father->mask = mask(brother->addr & brother->mask, ptr->base);
      __root = father;
    }
    else {
      if (brother->father->left == brother)
	brother->father->left = father;
      else
	brother->father->right = father;
      father->father = brother->father;

      /* necessary ? -- begin */
      aux = father;
      aux->mask = mask(aux->left->addr & aux->left->mask,
		       aux->right->addr & aux->right->mask);
      /* necessary ? -- end */
    }
    brother->father = father;
    if(!brother->is_leaf)
      brother->mask = mask(brother->left->addr & brother->left->mask,
			   brother->right->addr & brother->right->mask);

    assert((father->left == brother && father->right == new_leaf)
	   || (father->left == new_leaf && father->right == brother));
  }
}

__metadata_unit * __get_exact (void * ptr) {
  struct bittree * tmp = __root;
  assert(__root != NULL);
  assert(ptr != NULL);

  while(!tmp->is_leaf) {
    // prefix is consistent
    assert((tmp->addr & tmp->mask) == ((size_t)ptr & tmp->mask));
    // two sons
    assert(tmp->left != NULL && tmp->right != NULL);
    // the prefix of one son is consistent
    if((tmp->right->addr & tmp->right->mask)
       == ((size_t)ptr & tmp->right->mask))
      tmp = tmp->right;
    else if((tmp->left->addr & tmp->left->mask)
	    == ((size_t)ptr & tmp->left->mask))
      tmp = tmp->left;
    else
      assert(0);
  }

  assert(tmp->is_leaf);
  assert(tmp->leaf->base == (size_t)ptr);
  return tmp->leaf;
}

__metadata_unit * __get_cont (void * ptr) 
{
  struct bittree * tmp = __root;
  if(__root == NULL || ptr == NULL) return NULL;

  struct bittree * t [WORDBITS];
  short ind = -1;
  
  while(1) 
  {
    
    printf("leaf Traverse %p  mask %p\n",(void *)tmp->addr,(void *)tmp->mask);
    
    if(tmp->is_leaf) 
    {
      printf("leaf Traverse %p base %p bound %p mask %p\n",(void *)tmp->addr,(void *)tmp->leaf->base,(void *)tmp->leaf->bound,(void *)tmp->mask);
      /* tmp cannot contain ptr because its begin addr is higher */
      if(tmp->addr > (size_t)ptr) 
      {
	if(ind == -1)
	  return NULL;
	else 
        {
	  tmp = t[ind];
	  ind--;
	  continue;
	}
      }
      /* tmp->addr <= ptr, tmp may contain ptr
	 ptr is contained if tmp is large enough (begin addr + size) */
      else if((size_t)ptr <= tmp->leaf->bound)
	return tmp->leaf;
      /* tmp->addr <= ptr, but tmp->addr is not large enough */
      else if (ind == -1)
	return NULL;
      else 
      {
	tmp = t[ind];
	ind--;
	continue;
      }
    }
    
    assert(tmp->left != NULL && tmp->right != NULL);

    /* the right child has the highest address, so we test it first */
    if(((size_t)tmp->right->addr & tmp->right->mask) <= ((size_t)ptr & tmp->right->mask)) 
    {
//      printf("Traverse right %p  mask %p\n",(void *)tmp->right->addr,(void *)tmp->right->mask);
      ind++;
      t[ind] = tmp->left;
      tmp = tmp->right;
    }
    else if(((size_t)tmp->left->addr & tmp->left->mask) <= ((size_t)ptr & tmp->left->mask))
    {
//      printf("Traverse left %p  mask %p\n",(void *)tmp->left->addr,(void *)tmp->left->mask);
      tmp = tmp->left;
    }
    else 
    {
      if(ind == -1)
	return NULL;
      else 
      {
	tmp = t[ind];
	ind--;
      }
    }
  }
}

void traverse(struct bittree *node,int i,int deep)
{
	int j;
	j=deep;
	if(node!=NULL)
	{
		if(i==1)
		{	while(j-->0)
				printf(" ");
			printf("left %p  mask %p\n",(void *)node->addr,(void *)node->mask);
		}
		else if(i==2)
		{
			while(j-->0)
				printf(" ");
			printf("right %p  mask %p\n",(void *)node->addr,(void *)node->mask);
		}
		else
		{
			while(j-->0)
				printf(" ");
			printf("father %p  mask %p\n",(void *)node->addr,(void *)node->mask);
		}
		traverse(node->left,1,deep++);
		traverse(node->right,2,deep++);
	}
}
		
void show()
{
	struct bittree * tmp = __root;
	traverse(tmp,0,0);
}
