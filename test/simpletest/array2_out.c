#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
int tmp;
int t[10];
int u[8+3];
int v[16][17];
int array[16][17];

typedef struct _s {
  int t[15]; 
  struct {
    int u[12];
  }s;
  struct _s *next;
} ts;

ts s;

unsigned int c[10];
unsigned int var[10];

void __global_variables_init(){

__boundcheck_metadata_store((void *)(&tmp),(void *)((size_t)(&tmp)+sizeof(tmp)*8-1));
__boundcheck_metadata_store(&t[0],&t[10-1]);
__boundcheck_metadata_store(&u[0],&u[11-1]);

int __v_0;
	for(__v_0=0;__v_0<16;__v_0++)
__boundcheck_metadata_store(&v[__v_0][0],&v[__v_0][17-1]);

int __array_0;
	for(__array_0=0;__array_0<16;__array_0++)
__boundcheck_metadata_store(&array[__array_0][0],&array[__array_0][17-1]);

__boundcheck_metadata_store((void *)(&s),(void *)((size_t)(&s)+sizeof(s)*8-1));
__boundcheck_metadata_store(&c[0],&c[10-1]);
__boundcheck_metadata_store(&var[0],&var[10-1]);
}
int main() 
{
	__global_variables_init();


  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));

  unsigned int k;
__boundcheck_metadata_store((void *)(&k),(void *)((size_t)(&k)+sizeof(k)*8-1));

  i=j=k=1;
  t[_RV_insert_check(0,10,27,3,"main",0)] = 0;
  u[_RV_insert_check(0,11,28,3,"main",1)] = 0;
  v[_RV_insert_check(0,16,29,3,"main",2)][_RV_insert_check(0,17,29,3,"main",3)] = 0;
  v[_RV_insert_check(0,16,30,3,"main",k)][_RV_insert_check(0,17,30,3,"main",c[_RV_insert_check(0,10,30,8,"main",k)])] = 0;
  s.t[_RV_insert_check(0,15,31,3,"main",1)] = 0;
  s.s.u[_RV_insert_check(0,12,32,3,"main",2)] = 0;
  s.next->t[_RV_insert_check(0,15,33,3,"main",4)] = 0;

  t[_RV_insert_check(0,10,35,3,"main",i)] = 0;
  u[_RV_insert_check(0,11,36,3,"main",i)] = 0;
  v[_RV_insert_check(0,16,37,3,"main",i)][_RV_insert_check(0,17,37,3,"main",j)] = 0;
  s.t[_RV_insert_check(0,15,38,3,"main",i)] = 0;
  s.s.u[_RV_insert_check(0,12,39,3,"main",i)] = 0;
  s.next->t[_RV_insert_check(0,15,40,3,"main",j)] = 0;

  t[_RV_insert_check(0,10,42,3,"main",k)] = 0;
  u[_RV_insert_check(0,11,43,3,"main",k)] = 0;

  v[_RV_insert_check(0,16,45,3,"main",k)][_RV_insert_check(0,17,45,3,"main",c[_RV_insert_check(0,10,45,8,"main",k)])] = 0;


  array[_RV_insert_check(0,16,48,3,"main",k)][_RV_insert_check(0,17,48,3,"main",var[_RV_insert_check(0,10,48,12,"main",k)])]=0;
  v[_RV_insert_check(0,16,49,3,"main",k)][_RV_insert_check(0,17,49,3,"main",array[_RV_insert_check(0,16,49,8,"main",i)][_RV_insert_check(0,17,49,8,"main",j)])]=0;

  s.t[_RV_insert_check(0,15,51,3,"main",k)] = 0;
  s.s.u[_RV_insert_check(0,12,52,3,"main",k)] = 0;
  s.next->t[_RV_insert_check(0,15,53,3,"main",c[_RV_insert_check(0,10,53,13,"main",k)])] = 0;
  c[_RV_insert_check(0,10,54,3,"main",k)]=0;
  return 0;
}

