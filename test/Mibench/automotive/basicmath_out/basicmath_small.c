#include "snipmath.h"
#include <math.h>

/* The printf's may be removed to isolate just the math calculations */

int main(void)
{
  double  a1 = 1.0;
__boundcheck_metadata_store((void *)(&a1),(void *)((size_t)(&a1)+sizeof(a1)*8-1));
double  b1 = -10.5;
__boundcheck_metadata_store((void *)(&b1),(void *)((size_t)(&b1)+sizeof(b1)*8-1));
double  c1 = 32.0;
__boundcheck_metadata_store((void *)(&c1),(void *)((size_t)(&c1)+sizeof(c1)*8-1));
double  d1 = -30.0;
__boundcheck_metadata_store((void *)(&d1),(void *)((size_t)(&d1)+sizeof(d1)*8-1));

  double  a2 = 1.0;
__boundcheck_metadata_store((void *)(&a2),(void *)((size_t)(&a2)+sizeof(a2)*8-1));
double  b2 = -4.5;
__boundcheck_metadata_store((void *)(&b2),(void *)((size_t)(&b2)+sizeof(b2)*8-1));
double  c2 = 17.0;
__boundcheck_metadata_store((void *)(&c2),(void *)((size_t)(&c2)+sizeof(c2)*8-1));
double  d2 = -30.0;
__boundcheck_metadata_store((void *)(&d2),(void *)((size_t)(&d2)+sizeof(d2)*8-1));

  double  a3 = 1.0;
__boundcheck_metadata_store((void *)(&a3),(void *)((size_t)(&a3)+sizeof(a3)*8-1));
double  b3 = -3.5;
__boundcheck_metadata_store((void *)(&b3),(void *)((size_t)(&b3)+sizeof(b3)*8-1));
double  c3 = 22.0;
__boundcheck_metadata_store((void *)(&c3),(void *)((size_t)(&c3)+sizeof(c3)*8-1));
double  d3 = -31.0;
__boundcheck_metadata_store((void *)(&d3),(void *)((size_t)(&d3)+sizeof(d3)*8-1));

  double  a4 = 1.0;
__boundcheck_metadata_store((void *)(&a4),(void *)((size_t)(&a4)+sizeof(a4)*8-1));
double  b4 = -13.7;
__boundcheck_metadata_store((void *)(&b4),(void *)((size_t)(&b4)+sizeof(b4)*8-1));
double  c4 = 1.0;
__boundcheck_metadata_store((void *)(&c4),(void *)((size_t)(&c4)+sizeof(c4)*8-1));
double  d4 = -35.0;
__boundcheck_metadata_store((void *)(&d4),(void *)((size_t)(&d4)+sizeof(d4)*8-1));

  double  x[3];__boundcheck_metadata_store(&x[0],&x[3-1]);

  double X;
__boundcheck_metadata_store((void *)(&X),(void *)((size_t)(&X)+sizeof(X)*8-1));

  int     solutions;
__boundcheck_metadata_store((void *)(&solutions),(void *)((size_t)(&solutions)+sizeof(solutions)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

  unsigned long l = 0x3fed0169L;
__boundcheck_metadata_store((void *)(&l),(void *)((size_t)(&l)+sizeof(l)*8-1));

  struct int_sqrt q;
__boundcheck_metadata_store((void *)(&q),(void *)((size_t)(&q)+sizeof(q)*8-1));

  long n = 0;
__boundcheck_metadata_store((void *)(&n),(void *)((size_t)(&n)+sizeof(n)*8-1));


  /* solve soem cubic functions */
  printf("********* CUBIC FUNCTIONS ***********\n");
  /* should get 3 solutions: 2, 6 & 2.5   */
  SolveCubic(a1, b1, c1, d1, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,26,18,"main",i)]);
  printf("\n");
  /* should get 1 solution: 2.5           */
  SolveCubic(a2, b2, c2, d2, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,32,18,"main",i)]);
  printf("\n");
  SolveCubic(a3, b3, c3, d3, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,37,18,"main",i)]);
  printf("\n");
  SolveCubic(a4, b4, c4, d4, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,42,18,"main",i)]);
  printf("\n");
  /* Now solve some random equations */
  for(a1=1;a1<10;a1++) {
    for(b1=10;b1>0;b1--) {
      for(c1=5;c1<15;c1+=0.5) {
	for(d1=-1;d1>-11;d1--) {
	  SolveCubic(a1, b1, c1, d1, &solutions, x);  
	  printf("Solutions:");
	  for(i=0;i<solutions;i++)
	    printf(" %f",x[_RV_insert_check(0,3,52,19,"main",i)]);
	  printf("\n");
	}
      }
    }
  }
  
  printf("********* INTEGER SQR ROOTS ***********\n");
  /* perform some integer square roots */
  for (i = 0; i < 1001; ++i)
    {
      usqrt(i, &q);
			// remainder differs on some machines
     // printf("sqrt(%3d) = %2d, remainder = %2d\n",
     printf("sqrt(%3d) = %2d\n",
	     i, q.sqrt);
    }
  usqrt(l, &q);
  //printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);
  printf("\nsqrt(%lX) = %X\n", l, q.sqrt);


  printf("********* ANGLE CONVERSION ***********\n");
  /* convert some rads to degrees */
  for (X = 0.0; X <= 360.0; X += 1.0)
    printf("%3.0f degrees = %.12f radians\n", X, deg2rad(X));
  puts("");
  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180))
    printf("%.12f radians = %3.0f degrees\n", X, rad2deg(X));
  
  
  return 0;
}
