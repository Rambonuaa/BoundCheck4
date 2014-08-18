#include<stdio.h>
int a[3][3]={1, 2, 3, 4, 5, 6, 7, 8, 9};
int *b[]={a[0],a[1],a[2]};
int **p=b;
int main( )
{ 
   int i,j;
   for( i=0; i<3; i++ )
   for ( j=0; j<3; j++ )
       printf("%d,%d,%d\n", *(b[i]+j), *(*(p+i)+j), *(*(a+i)+j) );
   return 1;
}
