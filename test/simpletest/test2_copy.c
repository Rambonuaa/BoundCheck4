#include<stdio.h>
#define SIZE 5
typedef unsigned int size_t;
extern void e_acsl_assert(int predicate,char *kind,char *fct,char *pred_txt,int line);

extern  void *__store_block(void *ptr,size_t size);

extern  void __delete_block(void *ptr);

extern  void __initialize(void *ptr,size_t size);

extern  void __full_init(void *ptr);

extern  void __literal_string(void *ptr);

extern  void* __valid_read(void *ptr,size_t size);

extern  int __initialized(void *ptr,size_t size);

extern  void __e_acsl_memory_clean(void);

extern size_t __memory_size;

int main(void)
{
  char *__e_acsl_literal_string_5;
  char *__e_acsl_literal_string_4;
  char *__e_acsl_literal_string_3;
  char *__e_acsl_literal_string_2;
  char *__e_acsl_literal_string;
  char *pc[5];
  __store_block((void *)(pc),20U);
  char **p;
  __store_block((void *)(& p),4U);
  int i;
  
  __e_acsl_literal_string = " Beijing";
  __store_block((void *)__e_acsl_literal_string,sizeof(" Beijing"));
  pc[0] = (char *)__e_acsl_literal_string;

  __e_acsl_literal_string_2 = "Shanghaif";
  __store_block((void *)__e_acsl_literal_string_2,sizeof("Shanghaif"));
  pc[1] = (char *)__e_acsl_literal_string_2;

  __e_acsl_literal_string_3 = "Tianjingdfd";
  __store_block((void *)__e_acsl_literal_string_3,sizeof("Tianjingdfd"));
  pc[2] = (char *)__e_acsl_literal_string_3;

  __e_acsl_literal_string_4 = "Guangzhoudfgds";
  __store_block((void *)__e_acsl_literal_string_4,sizeof("Guangzhoudfgds"));
  pc[3] = (char *)__e_acsl_literal_string_4;

  __e_acsl_literal_string_5 = "Chongqingsdfsdf";
  __store_block((void *)__e_acsl_literal_string_5,sizeof("Chongqingsdfsdf"));
  pc[4] = (char *)__e_acsl_literal_string_5;

  for(i=0;i<SIZE;i++)
  {
     p = pc+i;
     printf ("%s\n", *((char **)__valid_read((void *)(p),sizeof(char *))));
//     printf ("%s\n", *p);
   printf("%c\n",*((char *)__valid_read((void *)(*((char **)__valid_read((void *)p,sizeof(char *))) + 20),sizeof(char))));
  }  
  return 0;
}


