
typedef unsigned int size_t;

extern  __attribute__((__FC_BUILTIN__)) void e_acsl_assert(int predicate,
                                                           char *kind,
                                                           char *fct,
                                                           char *pred_txt,
                                                           int line);


extern  __attribute__((__nothrow__)) void *__malloc(size_t size) __attribute__((
__malloc__, __leaf__));

/*@ assigns \result \from *((char *)ptr+(0..size-1)); */
extern  __attribute__((__FC_BUILTIN__)) void *__store_block(void *ptr,
                                                            size_t size);

/*@ assigns \nothing; */
extern  __attribute__((__FC_BUILTIN__)) void __delete_block(void *ptr);

/*@ assigns \nothing; */
extern  __attribute__((__FC_BUILTIN__)) void __initialize(void *ptr,
                                                          size_t size);

/*@ assigns \nothing; */
extern  __attribute__((__FC_BUILTIN__)) void __full_init(void *ptr);

/*@ assigns \nothing; */
extern  __attribute__((__FC_BUILTIN__)) void __literal_string(void *ptr);


extern  __attribute__((__FC_BUILTIN__)) int __valid_read(void *ptr,
                                                         size_t size);

extern  __attribute__((__FC_BUILTIN__)) void __e_acsl_memory_clean(void);


void *__e_acsl_malloc(size_t size)
{
  void *__retres;
  __store_block((void *)(& __retres),4U);
  __retres = __malloc(size);
  __delete_block((void *)(& __retres));
  return __retres;
}

extern int printf(char const * __restrict __format , ...);

extern  __attribute__((__nothrow__)) char *strcpy(char * __restrict __dest,
                                                  char const * __restrict __src) __attribute__((
__nonnull__(1,2), __leaf__));

int main(void)
{
  int i;
  char *a[10];
  __store_block((void *)(a),40U);
  i = 0;
  while (i < 10) {
    __initialize((void *)(& a[i]),sizeof(char *));
    a[i] = (char *)__e_acsl_malloc((unsigned int)100 * sizeof(char));
    i ++;
  }
  strcpy(a[5],"ASADAD");
  /*@ assert rte: mem_access: \valid_read(a[5]+1); */
  {
    int __e_acsl_valid_read;
    __e_acsl_valid_read = __valid_read((void *)(a[9] + 1),sizeof(char));
    e_acsl_assert(__e_acsl_valid_read,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(a[5]+1)",26);
  }
  printf("%c\n",*(a[5] + 1));
  
  __delete_block((void *)(a));
  __e_acsl_memory_clean();
  return 1;
}


