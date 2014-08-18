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
    printf(" %f",x[_RV_insert_check(0,3,23,18,"main",i)]);
  printf("\n");

  a1 = 1.0; b1 = -4.5; c1 = 17.0; d1 = -30.0;
  /* should get 1 solution: 2.5           */
  SolveCubic(a1, b1, c1, d1, &solutions, x);  
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,31,18,"main",i)]);
  printf("\n");

  a1 = 1.0; b1 = -3.5; c1 = 22.0; d1 = -31.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,38,18,"main",i)]);
  printf("\n");

  a1 = 1.0; b1 = -13.7; c1 = 1.0; d1 = -35.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,45,18,"main",i)]);
  printf("\n");

  a1 = 3.0; b1 = 12.34; c1 = 5.0; d1 = 12.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,52,18,"main",i)]);
  printf("\n");

  a1 = -8.0; b1 = -67.89; c1 = 6.0; d1 = -23.6;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,59,18,"main",i)]);
  printf("\n");

  a1 = 45.0; b1 = 8.67; c1 = 7.5; d1 = 34.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,66,18,"main",i)]);
  printf("\n");

  a1 = -12.0; b1 = -1.7; c1 = 5.3; d1 = 16.0;
  SolveCubic(a1, b1, c1, d1, &solutions, x);
  printf("Solutions:");
  for(i=0;i<solutions;i++)
    printf(" %f",x[_RV_insert_check(0,3,73,18,"main",i)]);
  printf("\n");

  /* Now solve some random equations */
  for(a1=1;a1<10;a1+=1) {
    for(b1=10;b1>0;b1-=.25) {
      for(c1=5;c1<15;c1+=0.61) {
	   for(d1=-1;d1>-5;d1-=.451) {
		SolveCubic(a1, b1, c1, d1, &solutions, x);  
		printf("Solutions:");
		for(i=0;i<solutions;i++)
		  printf(" %f",x[_RV_insert_check(0,3,84,18,"main",i)]);
		printf("\n");
	   }
      }
    }
  }


  printf("********* INTEGER SQR ROOTS ***********\n");
  /* perform some integer square roots */
  for (i = 0; i < 100000; i+=2)
    {
      usqrt(i, &q);
			// remainder differs on some machines
     // printf("sqrt(%3d) = %2d, remainder = %2d\n",
     printf("sqrt(%3d) = %2d\n",
	     i, q.sqrt);
    }
  printf("\n");
  for (l = 0x3fed0169L; l < 0x3fed4169L; l++)
    {
	 usqrt(l, &q);
	 //printf("\nsqrt(%lX) = %X, remainder = %X\n", l, q.sqrt, q.frac);
	 printf("sqrt(%lX) = %X\n", l, q.sqrt);
    }


  printf("********* ANGLE CONVERSION ***********\n");
  /* convert some rads to degrees */
/*   for (X = 0.0; X <= 360.0; X += 1.0) */
  for (X = 0.0; X <= 360.0; X += .001)
    printf("%3.0f degrees = %.12f radians\n", X, deg2rad(X));
  puts("");
/*   for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180)) */
  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 5760))
    printf("%.12f radians = %3.0f degrees\n", X, rad2deg(X));
  
  
  return 0;
}
