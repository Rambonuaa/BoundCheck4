/* Generated by Frama-C */
struct __anonstruct___mpz_struct_1 {
   int _mp_alloc ;
   int _mp_size ;
   unsigned long *_mp_d ;
};
typedef struct __anonstruct___mpz_struct_1 __mpz_struct;
typedef __mpz_struct ( __attribute__((__FC_BUILTIN__)) mpz_t)[1];
typedef unsigned int size_t;
typedef int wchar_t;
typedef unsigned int ino_t;
typedef unsigned int gid_t;
typedef unsigned int uid_t;
typedef long time_t;
typedef unsigned int blkcnt_t;
typedef unsigned int blksize_t;
typedef unsigned int dev_t;
typedef unsigned int mode_t;
typedef unsigned int nlink_t;
typedef unsigned int off_t;
struct stat {
   dev_t st_dev ;
   ino_t st_ino ;
   mode_t st_mode ;
   nlink_t st_nlink ;
   uid_t st_uid ;
   gid_t st_gid ;
   dev_t st_rdev ;
   off_t st_size ;
   time_t st_atime ;
   time_t st_mtime ;
   time_t st_ctime ;
   blksize_t st_blksize ;
   blkcnt_t st_blocks ;
   char *__fc_real_data ;
   int __fc_real_data_max_size ;
};
struct __fc_FILE {
   unsigned int __fc_stdio_id ;
   unsigned int __fc_maxsz ;
   unsigned int __fc_writepos ;
   unsigned int __fc_readpos ;
   int __fc_is_a_socket ;
   int mode ;
   struct stat *__fc_inode ;
};
typedef struct __fc_FILE FILE;
struct struct2 {
   int len ;
   char *string ;
   char string1[10] ;
   int interp ;
};
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

/*@ ensures \result ≡ 0 ∨ \result ≡ 1;
    ensures
      \result ≡ 1 ⇒ \initialized((char *)\old(ptr)+(0..\old(size)-1));
    assigns \result \from *((char *)ptr+(0..size-1));
 */
extern  __attribute__((__FC_BUILTIN__)) int __initialized(void *ptr,
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
extern FILE *__fc_stdout;

/*@ assigns *__fc_stdout;
    assigns *__fc_stdout \from *(format+(..)); */
extern int printf(char const *format , ...);

/*@
axiomatic
  MemCmp {
  logic ℤ memcmp{L}(char *s1, char *s2, ℤ n)
    
    reads *(s1+(0..n-1)), *(s2+(0..n-1));
  
  axiom
  memcmp_zero{L}:
                 ∀ char *s1, char *s2;
                   (∀ ℤ n;
                      memcmp{L}(s1, s2, n) ≡ 0 ⇔
                      (∀ ℤ i; 0 ≤ i ∧ i < n ⇒ *(s1+i) ≡ *(s2+i)));
  
  }
 */
/*@
axiomatic
  MemChr {
  logic 𝔹 memchr{L}(char *s, ℤ c, ℤ n) ;
  
  axiom
  memchr_def{L}:
                ∀ char *s;
                  (∀ ℤ c;
                     (∀ ℤ n;
                        memchr{L}(s, c, n) ≡ \true ⇔
                        (∃ int i; (0 ≤ i ∧ i < n) ∧ *(s+i) ≡ c)));
  
  }
 */
/*@
axiomatic
  MemSet {
  logic 𝔹 memset{L}(char *s, ℤ c, ℤ n) ;
  
  axiom
  memset_def{L}:
                ∀ char *s;
                  (∀ ℤ c;
                     (∀ ℤ n;
                        memset{L}(s, c, n) ≡ \true ⇔
                        (∀ ℤ i; 0 ≤ i ∧ i < n ⇒ *(s+i) ≡ c)));
  
  }
 */
/*@
axiomatic
  StrLen {
  logic ℤ strlen{L}(char *s) ;
  
  axiom
  strlen_pos_or_null{L}:
                        ∀ char *s;
                          (∀ ℤ i;
                             (0 ≤ i ∧
                              (∀ ℤ j;
                                 0 ≤ j ∧ j < i ⇒ *(s+j) ≢ '\000'))
                             ∧ *(s+i) ≡ '\000' ⇒ strlen{L}(s) ≡ i);
  
  axiom
  strlen_neg{L}:
                ∀ char *s;
                  (∀ ℤ i; 0 ≤ i ⇒ *(s+i) ≢ '\000') ⇒
                  strlen{L}(s) < 0;
  
  axiom
  strlen_before_null{L}:
                        ∀ char *s;
                          (∀ ℤ i;
                             0 ≤ i ∧ i < strlen{L}(s) ⇒
                             *(s+i) ≢ '\000');
  
  axiom
  strlen_at_null{L}:
                    ∀ char *s;
                      0 ≤ strlen{L}(s) ⇒ *(s+strlen{L}(s)) ≡ '\000';
  
  axiom
  strlen_not_zero{L}:
                     ∀ char *s;
                       (∀ ℤ i;
                          (0 ≤ i ∧ i ≤ strlen{L}(s)) ∧
                          *(s+i) ≢ '\000' ⇒ i < strlen{L}(s));
  
  axiom
  strlen_zero{L}:
                 ∀ char *s;
                   (∀ ℤ i;
                      (0 ≤ i ∧ i ≤ strlen{L}(s)) ∧ *(s+i) ≡ '\000'
                      ⇒ i ≡ strlen{L}(s));
  
  axiom
  strlen_sup{L}:
                ∀ char *s;
                  (∀ ℤ i;
                     0 ≤ i ∧ *(s+i) ≡ '\000' ⇒
                     0 ≤ strlen{L}(s) ∧ strlen{L}(s) ≤ i);
  
  axiom
  strlen_shift{L}:
                  ∀ char *s;
                    (∀ ℤ i;
                       0 ≤ i ∧ i ≤ strlen{L}(s) ⇒
                       strlen{L}(s+i) ≡ strlen{L}(s)-i);
  
  axiom
  strlen_create{L}:
                   ∀ char *s;
                     (∀ ℤ i;
                        0 ≤ i ∧ *(s+i) ≡ '\000' ⇒
                        0 ≤ strlen{L}(s) ∧ strlen{L}(s) ≤ i);
  
  axiom
  strlen_create_shift{L}:
                         ∀ char *s;
                           (∀ ℤ i;
                              (∀ ℤ k;
                                 (0 ≤ k ∧ k ≤ i) ∧ *(s+i) ≡ '\000'
                                 ⇒
                                 0 ≤ strlen{L}(s+k) ∧
                                 strlen{L}(s+k) ≤ i-k));
  
  axiom
  memcmp_strlen_left{L}:
                        ∀ char *s1, char *s2;
                          (∀ ℤ n;
                             memcmp{L}(s1, s2, n) ≡ 0 ∧ strlen{L}(s1) < n
                             ⇒ strlen{L}(s1) ≡ strlen{L}(s2));
  
  axiom
  memcmp_strlen_right{L}:
                         ∀ char *s1, char *s2;
                           (∀ ℤ n;
                              memcmp{L}(s1, s2, n) ≡ 0 ∧
                              strlen{L}(s2) < n ⇒
                              strlen{L}(s1) ≡ strlen{L}(s2));
  
  axiom
  memcmp_strlen_shift_left{L}:
                              ∀ char *s1, char *s2;
                                (∀ ℤ k, ℤ n;
                                   (memcmp{L}(s1, s2+k, n) ≡ 0 ∧ 0 ≤ k)
                                   ∧ strlen{L}(s1) < n ⇒
                                   0 ≤ strlen{L}(s2) ∧
                                   strlen{L}(s2) ≤ k+strlen{L}(s1));
  
  axiom
  memcmp_strlen_shift_right{L}:
                               ∀ char *s1, char *s2;
                                 (∀ ℤ k, ℤ n;
                                    (memcmp{L}(s1+k, s2, n) ≡ 0 ∧ 0 ≤ k)
                                    ∧ strlen{L}(s2) < n ⇒
                                    0 ≤ strlen{L}(s1) ∧
                                    strlen{L}(s1) ≤ k+strlen{L}(s2));
  
  }
 */
/*@
axiomatic
  StrCmp {
  logic ℤ strcmp{L}(char *s1, char *s2) ;
  
  axiom
  strcmp_zero{L}:
                 ∀ char *s1, char *s2;
                   strcmp{L}(s1, s2) ≡ 0 ⇔
                   strlen{L}(s1) ≡ strlen{L}(s2) ∧
                   (∀ ℤ i;
                      0 ≤ i ∧ i ≤ strlen{L}(s1) ⇒ *(s1+i) ≡ *(s2+i));
  
  }
 */
/*@
axiomatic
  StrNCmp {
  logic ℤ strncmp{L}(char *s1, char *s2, ℤ n) ;
  
  axiom
  strncmp_zero{L}:
                  ∀ char *s1, char *s2;
                    (∀ ℤ n;
                       strncmp{L}(s1, s2, n) ≡ 0 ⇔
                       (strlen{L}(s1) < n ∧ strcmp{L}(s1, s2) ≡ 0) ∨
                       (∀ ℤ i; 0 ≤ i ∧ i < n ⇒ *(s1+i) ≡ *(s2+i)));
  
  }
 */
/*@
axiomatic
  StrChr {
  logic 𝔹 strchr{L}(char *s, ℤ c) ;
  
  axiom
  strchr_def{L}:
                ∀ char *s;
                  (∀ ℤ c;
                     strchr{L}(s, c) ≡ \true ⇔
                     (∃ ℤ i;
                        (0 ≤ i ∧ i ≤ strlen{L}(s)) ∧ *(s+i) ≡ c));
  
  }
 */
/*@
axiomatic
  WcsLen {
  logic ℤ wcslen{L}(wchar_t *s) ;
  
  axiom
  wcslen_pos_or_null{L}:
                        ∀ wchar_t *s;
                          (∀ ℤ i;
                             (0 ≤ i ∧
                              (∀ ℤ j; 0 ≤ j ∧ j < i ⇒ *(s+j) ≢ 0))
                             ∧ *(s+i) ≡ 0 ⇒ wcslen{L}(s) ≡ i);
  
  axiom
  wcslen_neg{L}:
                ∀ wchar_t *s;
                  (∀ ℤ i; 0 ≤ i ⇒ *(s+i) ≢ 0) ⇒ wcslen{L}(s) < 0;
  
  axiom
  wcslen_before_null{L}:
                        ∀ wchar_t *s;
                          (∀ int i;
                             0 ≤ i ∧ i < wcslen{L}(s) ⇒ *(s+i) ≢ 0);
  
  axiom
  wcslen_at_null{L}:
                    ∀ wchar_t *s;
                      0 ≤ wcslen{L}(s) ⇒ *(s+wcslen{L}(s)) ≡ 0;
  
  axiom
  wcslen_not_zero{L}:
                     ∀ wchar_t *s;
                       (∀ int i;
                          (0 ≤ i ∧ i ≤ wcslen{L}(s)) ∧ *(s+i) ≢ 0
                          ⇒ i < wcslen{L}(s));
  
  axiom
  wcslen_zero{L}:
                 ∀ wchar_t *s;
                   (∀ int i;
                      (0 ≤ i ∧ i ≤ wcslen{L}(s)) ∧ *(s+i) ≡ 0 ⇒
                      i ≡ wcslen{L}(s));
  
  axiom
  wcslen_sup{L}:
                ∀ wchar_t *s;
                  (∀ int i;
                     0 ≤ i ∧ *(s+i) ≡ 0 ⇒
                     0 ≤ wcslen{L}(s) ∧ wcslen{L}(s) ≤ i);
  
  axiom
  wcslen_shift{L}:
                  ∀ wchar_t *s;
                    (∀ int i;
                       0 ≤ i ∧ i ≤ wcslen{L}(s) ⇒
                       wcslen{L}(s+i) ≡ wcslen{L}(s)-i);
  
  axiom
  wcslen_create{L}:
                   ∀ wchar_t *s;
                     (∀ int i;
                        0 ≤ i ∧ *(s+i) ≡ 0 ⇒
                        0 ≤ wcslen{L}(s) ∧ wcslen{L}(s) ≤ i);
  
  axiom
  wcslen_create_shift{L}:
                         ∀ wchar_t *s;
                           (∀ int i;
                              (∀ int k;
                                 (0 ≤ k ∧ k ≤ i) ∧ *(s+i) ≡ 0 ⇒
                                 0 ≤ wcslen{L}(s+k) ∧
                                 wcslen{L}(s+k) ≤ i-k));
  
  }
 */
/*@
axiomatic
  WcsCmp {
  logic ℤ wcscmp{L}(wchar_t *s1, wchar_t *s2) ;
  
  axiom
  wcscmp_zero{L}:
                 ∀ wchar_t *s1, wchar_t *s2;
                   wcscmp{L}(s1, s2) ≡ 0 ⇔
                   wcslen{L}(s1) ≡ wcslen{L}(s2) ∧
                   (∀ ℤ i;
                      0 ≤ i ∧ i ≤ wcslen{L}(s1) ⇒ *(s1+i) ≡ *(s2+i));
  
  }
 */
/*@
axiomatic
  WcsNCmp {
  logic ℤ wcsncmp{L}(wchar_t *s1, wchar_t *s2, ℤ n) ;
  
  axiom
  wcsncmp_zero{L}:
                  ∀ wchar_t *s1, wchar_t *s2;
                    (∀ ℤ n;
                       wcsncmp{L}(s1, s2, n) ≡ 0 ⇔
                       (wcslen{L}(s1) < n ∧ wcscmp{L}(s1, s2) ≡ 0) ∨
                       (∀ ℤ i; 0 ≤ i ∧ i < n ⇒ *(s1+i) ≡ *(s2+i)));
  
  }
 */
/*@ logic ℤ minimum(ℤ i, ℤ j) = i<j? i: j;
 */
/*@ logic ℤ maximum(ℤ i, ℤ j) = i<j? j: i;
 */
/*@
predicate valid_string{L}(char *s) =
  0 ≤ strlen{L}(s) ∧ \valid{L}(s+(0..strlen{L}(s)));
 */
/*@
predicate valid_string_or_null{L}(char *s) =
  s ≡ \null ∨ valid_string{L}(s);
 */
/*@
predicate valid_wstring{L}(wchar_t *s) =
  0 ≤ wcslen{L}(s) ∧ \valid{L}(s+(0..wcslen{L}(s)));
 */
/*@
predicate valid_wstring_or_null{L}(wchar_t *s) =
  s ≡ \null ∨ valid_wstring{L}(s);
 */
struct struct2 ss[8];
void __e_acsl_memory_init(void)
{
  char *__e_acsl_literal_string;
  __store_block((void *)(ss),192U);
  __e_acsl_literal_string = "undefine";
  __store_block((void *)__e_acsl_literal_string,sizeof("undefine"));
  __full_init((void *)__e_acsl_literal_string);
  __literal_string((void *)__e_acsl_literal_string);
  ss = (char *)__e_acsl_literal_string;
  return;
}

int main(void)
{
  char *__e_acsl_literal_string_2;
  char *__e_acsl_literal_string;
  int __retres;
  struct struct2 *p;
  char *q;
  char **l;
  __e_acsl_memory_init();
  __store_block((void *)(& l),4U);
  __store_block((void *)(& q),4U);
  __store_block((void *)(& p),4U);
  __full_init((void *)(& p));
  p = ss;
  /*@ assert rte: mem_access: \valid_read(&(p+2)->string); */
  {
    int __e_acsl_valid_read;
    __e_acsl_valid_read = __valid_read((void *)(& (p + 2)->string),
                                       sizeof(char *));
    e_acsl_assert(__e_acsl_valid_read,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(&(p+2)->string)",23);
    __full_init((void *)(& q));
    q = (p + 2)->string + 0;
  }
  __full_init((void *)(& l));
  l = & q;
  /*@ assert rte: mem_access: \valid_read(l); */
  {
    int __e_acsl_initialized;
    int __e_acsl_and;
    __e_acsl_initialized = __initialized((void *)(& l),sizeof(char **));
    if (__e_acsl_initialized) {
      int __e_acsl_valid_read_2;
      __e_acsl_valid_read_2 = __valid_read((void *)l,sizeof(char *));
      __e_acsl_and = __e_acsl_valid_read_2;
    }
    else __e_acsl_and = 0;
    e_acsl_assert(__e_acsl_and,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(l)",25);
    __e_acsl_literal_string = "%s\n";
    __store_block((void *)__e_acsl_literal_string,sizeof("%s\n"));
    __full_init((void *)__e_acsl_literal_string);
    __literal_string((void *)__e_acsl_literal_string);
    printf(__e_acsl_literal_string,*l);
  }
  /*@ assert rte: mem_access: \valid_read(l); */
  /*@ assert rte: mem_access: \valid_read(*l); */
  {
    int __e_acsl_initialized_2;
    int __e_acsl_and_3;
    int __e_acsl_initialized_4;
    int __e_acsl_and_4;
    __e_acsl_initialized_2 = __initialized((void *)l,sizeof(char *));
    if (__e_acsl_initialized_2) {
      int __e_acsl_initialized_3;
      int __e_acsl_and_2;
      int __e_acsl_valid_read_4;
      __e_acsl_initialized_3 = __initialized((void *)(& l),sizeof(char **));
      if (__e_acsl_initialized_3) {
        int __e_acsl_valid_read_3;
        __e_acsl_valid_read_3 = __valid_read((void *)l,sizeof(char *));
        __e_acsl_and_2 = __e_acsl_valid_read_3;
      }
      else __e_acsl_and_2 = 0;
      e_acsl_assert(__e_acsl_and_2,(char *)"RTE",(char *)"main",
                    (char *)"mem_access: \\valid_read(l)",26);
      __e_acsl_valid_read_4 = __valid_read((void *)*l,sizeof(char));
      __e_acsl_and_3 = __e_acsl_valid_read_4;
    }
    else __e_acsl_and_3 = 0;
    e_acsl_assert(__e_acsl_and_3,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(*l)",26);
    __e_acsl_initialized_4 = __initialized((void *)(& l),sizeof(char **));
    if (__e_acsl_initialized_4) {
      int __e_acsl_valid_read_5;
      __e_acsl_valid_read_5 = __valid_read((void *)l,sizeof(char *));
      __e_acsl_and_4 = __e_acsl_valid_read_5;
    }
    else __e_acsl_and_4 = 0;
    e_acsl_assert(__e_acsl_and_4,(char *)"Assertion",(char *)"main",
                  (char *)"rte: mem_access: \\valid_read(l)",26);
    __e_acsl_literal_string_2 = "%c\n";
    __store_block((void *)__e_acsl_literal_string_2,sizeof("%c\n"));
    __full_init((void *)__e_acsl_literal_string_2);
    __literal_string((void *)__e_acsl_literal_string_2);
    printf(__e_acsl_literal_string_2,*(*l));
  }
  __retres = 1;
  __delete_block((void *)(ss));
  __delete_block((void *)(& l));
  __delete_block((void *)(& q));
  __delete_block((void *)(& p));
  __e_acsl_memory_clean();
  return __retres;
}


