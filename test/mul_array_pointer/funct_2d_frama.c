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

/*@
predicate diffSize{L1, L2}(ℤ i) =
  \at(__memory_size,L1)-\at(__memory_size,L2) ≡ i;
 */
/*@ assigns \at(\result,Post) \from *__format; */
extern int printf(char const * __restrict __format , ...);

void show_2d_array(int (*array)[10], int rows)
{
  char *__e_acsl_literal_string;
  int i;
  int j;
  __store_block((void *)(& array),4U);
  i = 0;
  while (i < rows) {
    j = 0;
    while (j < 10) {
      /*@ assert rte: index_bound: 0 ≤ j; */
      e_acsl_assert(0 <= j,(char *)"Assertion",(char *)"show_2d_array",
                    (char *)"rte: index_bound: 0 <= j",12);
      /*@ assert rte: index_bound: j < 10; */
      e_acsl_assert(j < 10,(char *)"Assertion",(char *)"show_2d_array",
                    (char *)"rte: index_bound: j < 10",13);
      /*@ assert rte: mem_access: \valid_read(&(*(array+i))[j]); */
      {
        int __e_acsl_valid_read;
        __e_acsl_valid_read = __valid_read((void *)(& (*(array + i))[j]),
                                           sizeof(int));
        e_acsl_assert(__e_acsl_valid_read,(char *)"Assertion",
                      (char *)"show_2d_array",
                      (char *)"rte: mem_access: \\valid_read(&(*(array+i))[j])",
                      14);
      }
      __e_acsl_literal_string = "array[%d][%d] = %d\n";
      __store_block((void *)__e_acsl_literal_string,
                    sizeof("array[%d][%d] = %d\n"));
      __full_init((void *)__e_acsl_literal_string);
      __literal_string((void *)__e_acsl_literal_string);
      printf(__e_acsl_literal_string,i,j,(*(array + i))[j]);
      /*@ assert rte: signed_overflow: j+1 ≤ 2147483647; */
      e_acsl_assert((long long)j + (long long)1 <= (long long)2147483647,
                    (char *)"Assertion",(char *)"show_2d_array",
                    (char *)"rte: signed_overflow: j+1 <= 2147483647",16);
      j ++;
    }
    /*@ assert rte: signed_overflow: i+1 ≤ 2147483647; */
    e_acsl_assert((long long)i + (long long)1 <= (long long)2147483647,
                  (char *)"Assertion",(char *)"show_2d_array",
                  (char *)"rte: signed_overflow: i+1 <= 2147483647",19);
    i ++;
  }
  __delete_block((void *)(& array));
  return;
}

int main(void)
{
  int __retres;
  int a[1][10];
  int b[2][10];
  int c[3][10];
  __store_block((void *)(c),120U);
  __store_block((void *)(b),80U);
  __store_block((void *)(a),40U);
  __initialize((void *)(& a[0][0]),sizeof(int));
  a[0][0] = 1;
  __initialize((void *)(& a[0][1]),sizeof(int));
  a[0][1] = 2;
  __initialize((void *)(& a[0][2]),sizeof(int));
  a[0][2] = 3;
  __initialize((void *)(& a[0][3]),sizeof(int));
  a[0][3] = 4;
  __initialize((void *)(& a[0][4]),sizeof(int));
  a[0][4] = 5;
  __initialize((void *)(& a[0][5]),sizeof(int));
  a[0][5] = 6;
  __initialize((void *)(& a[0][6]),sizeof(int));
  a[0][6] = 7;
  __initialize((void *)(& a[0][7]),sizeof(int));
  a[0][7] = 8;
  __initialize((void *)(& a[0][8]),sizeof(int));
  a[0][8] = 9;
  __initialize((void *)(& a[0][9]),sizeof(int));
  a[0][9] = 10;
  __initialize((void *)(& b[0][0]),sizeof(int));
  b[0][0] = 1;
  __initialize((void *)(& b[0][1]),sizeof(int));
  b[0][1] = 2;
  __initialize((void *)(& b[0][2]),sizeof(int));
  b[0][2] = 3;
  __initialize((void *)(& b[0][3]),sizeof(int));
  b[0][3] = 4;
  __initialize((void *)(& b[0][4]),sizeof(int));
  b[0][4] = 5;
  __initialize((void *)(& b[0][5]),sizeof(int));
  b[0][5] = 6;
  __initialize((void *)(& b[0][6]),sizeof(int));
  b[0][6] = 7;
  __initialize((void *)(& b[0][7]),sizeof(int));
  b[0][7] = 8;
  __initialize((void *)(& b[0][8]),sizeof(int));
  b[0][8] = 9;
  __initialize((void *)(& b[0][9]),sizeof(int));
  b[0][9] = 10;
  __initialize((void *)(& b[1][0]),sizeof(int));
  b[1][0] = 11;
  __initialize((void *)(& b[1][1]),sizeof(int));
  b[1][1] = 12;
  __initialize((void *)(& b[1][2]),sizeof(int));
  b[1][2] = 13;
  __initialize((void *)(& b[1][3]),sizeof(int));
  b[1][3] = 14;
  __initialize((void *)(& b[1][4]),sizeof(int));
  b[1][4] = 15;
  __initialize((void *)(& b[1][5]),sizeof(int));
  b[1][5] = 16;
  __initialize((void *)(& b[1][6]),sizeof(int));
  b[1][6] = 17;
  __initialize((void *)(& b[1][7]),sizeof(int));
  b[1][7] = 18;
  __initialize((void *)(& b[1][8]),sizeof(int));
  b[1][8] = 19;
  __initialize((void *)(& b[1][9]),sizeof(int));
  b[1][9] = 20;
  __initialize((void *)(& c[0][0]),sizeof(int));
  c[0][0] = 1;
  __initialize((void *)(& c[0][1]),sizeof(int));
  c[0][1] = 2;
  __initialize((void *)(& c[0][2]),sizeof(int));
  c[0][2] = 3;
  __initialize((void *)(& c[0][3]),sizeof(int));
  c[0][3] = 4;
  __initialize((void *)(& c[0][4]),sizeof(int));
  c[0][4] = 5;
  __initialize((void *)(& c[0][5]),sizeof(int));
  c[0][5] = 6;
  __initialize((void *)(& c[0][6]),sizeof(int));
  c[0][6] = 7;
  __initialize((void *)(& c[0][7]),sizeof(int));
  c[0][7] = 8;
  __initialize((void *)(& c[0][8]),sizeof(int));
  c[0][8] = 9;
  __initialize((void *)(& c[0][9]),sizeof(int));
  c[0][9] = 10;
  __initialize((void *)(& c[1][0]),sizeof(int));
  c[1][0] = 11;
  __initialize((void *)(& c[1][1]),sizeof(int));
  c[1][1] = 12;
  __initialize((void *)(& c[1][2]),sizeof(int));
  c[1][2] = 13;
  __initialize((void *)(& c[1][3]),sizeof(int));
  c[1][3] = 14;
  __initialize((void *)(& c[1][4]),sizeof(int));
  c[1][4] = 15;
  __initialize((void *)(& c[1][5]),sizeof(int));
  c[1][5] = 16;
  __initialize((void *)(& c[1][6]),sizeof(int));
  c[1][6] = 17;
  __initialize((void *)(& c[1][7]),sizeof(int));
  c[1][7] = 18;
  __initialize((void *)(& c[1][8]),sizeof(int));
  c[1][8] = 19;
  __initialize((void *)(& c[1][9]),sizeof(int));
  c[1][9] = 20;
  __initialize((void *)(& c[2][0]),sizeof(int));
  c[2][0] = 21;
  __initialize((void *)(& c[2][1]),sizeof(int));
  c[2][1] = 22;
  __initialize((void *)(& c[2][2]),sizeof(int));
  c[2][2] = 23;
  __initialize((void *)(& c[2][3]),sizeof(int));
  c[2][3] = 24;
  __initialize((void *)(& c[2][4]),sizeof(int));
  c[2][4] = 25;
  __initialize((void *)(& c[2][5]),sizeof(int));
  c[2][5] = 26;
  __initialize((void *)(& c[2][6]),sizeof(int));
  c[2][6] = 27;
  __initialize((void *)(& c[2][7]),sizeof(int));
  c[2][7] = 28;
  __initialize((void *)(& c[2][8]),sizeof(int));
  c[2][8] = 29;
  __initialize((void *)(& c[2][9]),sizeof(int));
  c[2][9] = 30;
  show_2d_array(a,1);
  show_2d_array(b,2);
  show_2d_array(c,3);
  __retres = 1;
  __delete_block((void *)(c));
  __delete_block((void *)(b));
  __delete_block((void *)(a));
  __e_acsl_memory_clean();
  return __retres;
}


