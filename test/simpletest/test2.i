/* Generated by Frama-C */
extern int printf(char const * __restrict __format , ...);

int main(void)
{
  int __retres;
  char *pc[5];
  char **p;
  int i;
  pc[0] = (char *)" Beijing";
  pc[1] = (char *)"Shanghai";
  pc[2] = (char *)"Tianjing";
  pc[3] = (char *)"Guangzhou";
  pc[4] = (char *)"Chongqing";
  i = 0;
  while (i < 5) {
    p = & pc[i];
    /*@ assert rte: mem_access: \valid_read(p); */
    printf("%s\n",*p);
    /*@ assert rte: mem_access: \valid_read(p); */
    /*@ assert rte: mem_access: \valid_read(*p+2); */
    printf("%c\n",*(*p + 2));
    /*@ assert rte: signed_overflow: i+1 ≤ 2147483647; */
    i ++;
  }
  __retres = 0;
  return __retres;
}

