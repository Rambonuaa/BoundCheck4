#include"boundcheck.h"
#include<string.h>
/* run.config
   OPT: -rte -warn-signed-overflow -print -machdep x86_32
*/
#include<stdio.h>
int main() 
{
  int x=1;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));

  printf("x=%p \n",&x);
  int *p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));
int * q;
__boundcheck_metadata_store((void *)(&q),(void *)((size_t)(&q)+sizeof(q)*8-1));

  printf("p=%p \n",(void *)p);
  int tab[10]={0};
__boundcheck_metadata_store(&tab[0],&tab[10-1]);


  p = &x;
  printf("p--=%p \n",(void *)p);
  q = p;
__boundcheck_metadata_trans_check((void *)(q),(void *)(p),(void *)(p));

  *(int *)(__boundcheck_ptr_reference(16,4,"main",(void *)(q),(void *)(q))) = *(int *)(__boundcheck_ptr_reference(16,9,"main",(void *)(p),(void *)(p))) + 5;

  tab[_RV_insert_check(0,10,18,3,"main",3)] = *(int *)(__boundcheck_ptr_reference(18,13,"main",(void *)(q),(void *)(q)));
  tab[_RV_insert_check(0,10,19,3,"main",x)] = *(int *)(__boundcheck_ptr_reference(19,13,"main",(void *)(q),(void *)(q)));

  p = &tab[_RV_insert_check(0,10,21,8,"main",2)];
  p = &tab[_RV_insert_check(0,10,22,8,"main",x)];

  *(int *)(__boundcheck_ptr_reference(24,8,"main",(void *)(p),(void *)(p+1))) = tab[_RV_insert_check(0,10,24,12,"main",0)];
  *(int *)(__boundcheck_ptr_reference(25,8,"main",(void *)(p),(void *)(p+1))) = tab[_RV_insert_check(0,10,25,12,"main",x)];

  *(int *)(__boundcheck_ptr_reference(27,4,"main",(void *)(q),(void *)(q)))=p[2];

  return 0;
}
