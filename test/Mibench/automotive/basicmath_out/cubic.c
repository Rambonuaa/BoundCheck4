/* +++Date last modified: 05-Jul-1997 */

/*
**  CUBIC.C - Solve a cubic polynomial
**  public domain by Ross Cottrell
*/

#include <math.h>
#include <stdlib.h>
#include "snipmath.h"

void SolveCubic(double  a,
                double  b,
                double  c,
                double  d,
                int    *solutions,
                double *x)
{
      long double    a1 = b/a;
__boundcheck_metadata_store((void *)(&a1),(void *)((size_t)(&a1)+sizeof(a1)*8-1));
long double  a2 = c/a;
__boundcheck_metadata_store((void *)(&a2),(void *)((size_t)(&a2)+sizeof(a2)*8-1));
long double  a3 = d/a;
__boundcheck_metadata_store((void *)(&a3),(void *)((size_t)(&a3)+sizeof(a3)*8-1));

      long double    Q = (a1*a1 - 3.0*a2)/9.0;
__boundcheck_metadata_store((void *)(&Q),(void *)((size_t)(&Q)+sizeof(Q)*8-1));

      long double R = (2.0*a1*a1*a1 - 9.0*a1*a2 + 27.0*a3)/54.0;
__boundcheck_metadata_store((void *)(&R),(void *)((size_t)(&R)+sizeof(R)*8-1));

      double    R2_Q3 = R*R - Q*Q*Q;
__boundcheck_metadata_store((void *)(&R2_Q3),(void *)((size_t)(&R2_Q3)+sizeof(R2_Q3)*8-1));


      double    theta;
__boundcheck_metadata_store((void *)(&theta),(void *)((size_t)(&theta)+sizeof(theta)*8-1));


      if (R2_Q3 <= 0)
      {
            *(int *)(__boundcheck_ptr_reference(28,14,"SolveCubic",(void *)(solutions),(void *)(solutions))) = 3;
            theta = acos(R/sqrt(Q*Q*Q));
            (*(double *)(__boundcheck_ptr_reference(30,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[0])))) = -2.0*sqrt(Q)*cos(theta/3.0) - a1/3.0;
            (*(double *)(__boundcheck_ptr_reference(31,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[1])))) = -2.0*sqrt(Q)*cos((theta+2.0*PI)/3.0) - a1/3.0;
            (*(double *)(__boundcheck_ptr_reference(32,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[2])))) = -2.0*sqrt(Q)*cos((theta+4.0*PI)/3.0) - a1/3.0;
      }
      else
      {
            *(int *)(__boundcheck_ptr_reference(36,14,"SolveCubic",(void *)(solutions),(void *)(solutions))) = 1;
            (*(double *)(__boundcheck_ptr_reference(37,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[0])))) = pow(sqrt(R2_Q3)+fabs(R), 1/3.0);
            (*(double *)(__boundcheck_ptr_reference(38,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[0])))) += Q/(*(double *)(__boundcheck_ptr_reference(38,23,"SolveCubic",(void *)(&x[0]),(void *)(&x[0]))));
            (*(double *)(__boundcheck_ptr_reference(39,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[0])))) *= (R < 0.0) ? 1 : -1;
            (*(double *)(__boundcheck_ptr_reference(40,13,"SolveCubic",(void *)(&x[0]),(void *)(&x[0])))) -= a1/3.0;
      }
}

#ifdef TEST

int main(void)
{
      double  a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
      double  a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
      double  x[3];
      int     solutions;

      SolveCubic(a1, b1, c1, d1, &solutions, x);

      /* should get 3 solutions: 2, 6 & 2.5   */

      SolveCubic(a2, b2, c2, d2, &solutions, x);

      /* should get 1 solution: 2.5           */

      return 0;
}

#endif /* TEST */
