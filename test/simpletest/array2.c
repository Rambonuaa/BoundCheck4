#include<stdio.h>
int tmp;
int t[10];
int u[8+3];
int v[16][17];
int array[16][17];

typedef struct _s {
  int t[15]; 
  struct {
    int u[12];
  }s;
  struct _s *next;
} ts;

ts s;

unsigned int c[10];
unsigned int var[10];

int main() 
{

  int i,j;
  unsigned int k;
  i=j=k=1;
  t[0] = 0;
  u[1] = 0;
  v[2][3] = 0;
  v[k][c[k]] = 0;
  s.t[1] = 0;
  s.s.u[2] = 0;
  s.next->t[4] = 0;

  t[i] = 0;
  u[i] = 0;
  v[i][j] = 0;
  s.t[i] = 0;
  s.s.u[i] = 0;
  s.next->t[j] = 0;

  t[k] = 0;
  u[k] = 0;

  v[k][c[k]] = 0;


  array[k][var[k]]=0;
  v[k][array[i][j]]=0;

  s.t[k] = 0;
  s.s.u[k] = 0;
  s.next->t[c[k]] = 0;
  c[k]=0;
  return 0;
}

