typedef struct{
    int    Nkey;               /* the number of words in the key input block */
    int    Nrnd;               /* the number of cipher rounds                */
    int    e_key[100];   /* the encryption key schedule                */
    int    d_key[100];   /* the decryption key schedule                */
} c_name;
struct {
int a;
int array[100];
}s;

#include<stdio.h>
int main()
{
   int *p1;
   int *p2;
   int *p3;
   c_name *cx;
   int *ky;
   ky=cx->e_key;
   printf("%d\n",ky[_RV_insert_check(0,100,21,21,"main",102)]);

// uninitialized pointer dereference
   printf("%d\n",*(int *)(_RV_insert_pointer_check((void *)p1,0,24,19,"main",(void *)p1)));

// null pointer dereference
   p2=NULL;
   printf("%d\n",*(int *)(_RV_insert_pointer_check((void *)p2,0,28,19,"main",(void *)p2)));

// manufactured pointer dereference
   p3=(int *) 42;
   printf("%d\n",*(int *)(_RV_insert_pointer_check((void *)p3,0,32,19,"main",(void *)p3)));

   return 0;
}

