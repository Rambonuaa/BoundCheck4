/* Generated by Frama-C */
struct __anonstruct___mpz_struct_1 {
   int _mp_alloc ;
   int _mp_size ;
   unsigned long *_mp_d ;
};
typedef struct __anonstruct___mpz_struct_1 __mpz_struct;
typedef __mpz_struct ( __attribute__((__FC_BUILTIN__)) mpz_t)[1];
typedef unsigned int size_t;
/*@ requires predicate ≢ 0;
    assigns \nothing; */
extern  __attribute__((__FC_BUILTIN__)) void e_acsl_assert(int predicate,
                                                           char *kind,
                                                           char *fct,
                                                           char *pred_txt,
                                                           int line);

/*@
model __mpz_struct { ℤ n };
*/
int __fc_random_counter __attribute__((__unused__));
unsigned long const __fc_rand_max = (unsigned long)32767;
/*@ ghost extern int __fc_heap_status; */

/*@
axiomatic
  dynamic_allocation {
  predicate is_allocable{L}(size_t n) 
    reads __fc_heap_status;
  
  }
 */
/*@ assigns __fc_heap_status;
    assigns __fc_heap_status \from size, __fc_heap_status;
    assigns \result \from size, __fc_heap_status;
    allocates \result;
    
    behavior allocation:
      assumes is_allocable(size);
      ensures \fresh{Old, Here}(\result,\old(size));
      assigns __fc_heap_status;
      assigns __fc_heap_status \from size, __fc_heap_status;
      assigns \result \from size, __fc_heap_status;
    
    behavior no_allocation:
      assumes ¬is_allocable(size);
      ensures \result ≡ \null;
      assigns \result \from \nothing;
      allocates \nothing;
    
    complete behaviors no_allocation, allocation;
    disjoint behaviors no_allocation, allocation;
 */
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

/*@ ensures \result ≡ 0 ∨ \result ≡ 1;
    ensures
      \result ≡ 1 ⇒ \valid_read((char *)\old(ptr)+(0..\old(size)-1));
    assigns \result \from *((char *)ptr+(0..size-1));
 */
extern  __attribute__((__FC_BUILTIN__)) int __valid_read(void *ptr,
                                                         size_t size);

/*@ assigns __e_acsl_internal_heap;
    assigns __e_acsl_internal_heap \from __e_acsl_internal_heap;
 */
extern  __attribute__((__FC_BUILTIN__)) void __e_acsl_memory_clean(void);

extern size_t __memory_size;

/*@ assigns __fc_heap_status;
    assigns __fc_heap_status \from size, __fc_heap_status;
    assigns \result \from size, __fc_heap_status;
    allocates \result;
    
    behavior allocation:
      assumes is_allocable(size);
      ensures \fresh{Old, Here}(\result,\old(size));
      assigns __fc_heap_status;
      assigns __fc_heap_status \from size, __fc_heap_status;
      assigns \result \from size, __fc_heap_status;
    
    behavior no_allocation:
      assumes ¬is_allocable(size);
      ensures \result ≡ \null;
      assigns \result \from \nothing;
      allocates \nothing;
    
    complete behaviors no_allocation, allocation;
    disjoint behaviors no_allocation, allocation;
 */
void *__e_acsl_malloc(size_t size)
{
  void *__retres;
  __store_block((void *)(& __retres),4U);
  __retres = __malloc(size);
  __delete_block((void *)(& __retres));
  return __retres;
}

/*@
predicate diffSize{L1, L2}(ℤ i) =
  \at(__memory_size,L1)-\at(__memory_size,L2) ≡ i;

*/
/*@ assigns \at(\result,Post) \from *__format; */
extern int printf(char const * __restrict __format , ...);

/*@ assigns *__dest;
    assigns \at(\result,Post) \from *__dest, *__src;
    assigns *__dest \from *__dest, *__src;
 */
extern  __attribute__((__nothrow__)) char *strcpy(char * __restrict __dest,
                                                  char const * __restrict __src) __attribute__((
__nonnull__(1,2), __leaf__));

int main(void)
{
  char *__e_acsl_literal_string_2;
  char *__e_acsl_literal_string;
  int __retres;
  int i;
  char a[6];
  char *p;
  __store_block((void *)(& p),4U);
  __store_block((void *)(a),6U);
  i = 0;
  __initialize((void *)(a),sizeof(char));
  a[0] = (char)'a';
  __initialize((void *)(& a[1]),sizeof(char));
  a[1] = (char)'b';
  __initialize((void *)(& a[2]),sizeof(char));
  a[2] = (char)'c';
  __initialize((void *)(& a[3]),sizeof(char));
  a[3] = (char)'d';
  __initialize((void *)(& a[4]),sizeof(char));
  a[4] = (char)'e';
  __initialize((void *)(& a[5]),sizeof(char));
  a[5] = (char)'U';
  if (i) {
    __full_init((void *)(& p));
    p = (char *)__e_acsl_malloc((unsigned int)7 * sizeof(char));
    __e_acsl_literal_string = "ASDFFG";
    __store_block((void *)__e_acsl_literal_string,sizeof("ASDFFG"));
    __full_init((void *)__e_acsl_literal_string);
    __literal_string((void *)__e_acsl_literal_string);
    strcpy(p,__e_acsl_literal_string);
  }
  else {
    __full_init((void *)(& p));
    p = a;
  }
  /*@ assert rte: mem_access: \valid_read(p+4); */
  {
    int __e_acsl_valid_read;
    __e_acsl_valid_read = __valid_read((void *)(p + 4),sizeof(char));
    e_acsl_assert(__e_acsl_valid_read,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(p+4)",30);
  }
  __e_acsl_literal_string_2 = "%c\n";
  __store_block((void *)__e_acsl_literal_string_2,sizeof("%c\n"));
  __full_init((void *)__e_acsl_literal_string_2);
  __literal_string((void *)__e_acsl_literal_string_2);
  printf(__e_acsl_literal_string_2,*(p + 4));
  __retres = 1;
  __delete_block((void *)(& p));
  __delete_block((void *)(a));
  __e_acsl_memory_clean();
  return __retres;
}

