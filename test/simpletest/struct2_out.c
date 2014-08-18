#include"self.h"
#include<string.h>
/* run.config
   OPT: -rte -warn-signed-overflow -print -machdep x86_32 -then -unsafe-arrays
*/

struct R {
  int v;
};

struct Q {
  int v;
  int id[12];
  struct P* next;
  struct R tr[13];
};

struct P {
  int val;
  struct P* next;
  int id[5];
  int oth[6][7];
  struct P* nexts[8][9];
  struct Q q;
  struct Q tq[10][11];
  struct P*** znexts;
};

int main() {
  struct P*** pppp;
void *_pppp_base=pppp;
void *_pppp_bound=NULL;

  struct P** ppp;
void *_ppp_base=ppp;
void *_ppp_bound=NULL;

  struct P* pp;
void *_pp_base=pp;
void *_pp_bound=NULL;

  struct P p, p2;
  int v;
  struct Q q;
  int *i;
void *_i_base=i;
void *_i_bound=NULL;
j;
  int i0 = 0;
  int i1 = 1;
  int i2 = 2;
  int i3 = 3;
  int i4 = 4;
  j=0;
  i = &j;
  pp = &p;
  ppp = &pp;
  pppp = &ppp;
  p.next = pp;
  p.znexts = pppp;
  p.nexts[_RV_insert_check(0,8,47,11,"main",i0)][_RV_insert_check(0,9,47,15,"main",i1)] = p.nexts[_RV_insert_check(0,8,47,29,"main",i2)][_RV_insert_check(0,9,47,33,"main",i3)] = pp;
  q.next = pp;
  q.id[_RV_insert_check(0,12,49,8,"main",i0)] = 0;
  p.q = q;
  p.tq[_RV_insert_check(0,10,51,8,"main",i0)][_RV_insert_check(0,11,51,12,"main",i1)] = q;
  p.id[_RV_insert_check(0,5,52,8,"main",i1)] = p.id[_RV_insert_check(0,5,52,19,"main",i2)];
  p.id[_RV_insert_check(0,5,53,8,"main",i1)] = p.id[_RV_insert_check(0,5,53,19,"main",i3)];

  struct P np = *(p.next);
  struct P* npp = p.next;

  v = p.id[_RV_insert_check(0,5,58,12,"main",3)];
  v = pp->id[_RV_insert_check(0,5,59,14,"main",3)];

  
  v = *(int *)(_RV_insert_pointer_check(_i_base,_i_bound,62,8,"main",(void *)i));
  v = pp->val;
  v = pp->id[_RV_insert_check(0,5,64,14,"main",3)];
  struct P* z = pp->nexts[_RV_insert_check(0,8,65,27,"main",i0)][_RV_insert_check(0,9,65,31,"main",i1)];

  v = pp->nexts[_RV_insert_check(0,8,67,17,"main",i0)][_RV_insert_check(0,9,67,21,"main",i1)]->val;
  v = pp->next->val;
  v = pp->next->next->val;
  v = p.nexts[_RV_insert_check(0,8,70,15,"main",i0)][_RV_insert_check(0,9,70,19,"main",i1)]->val;
  v = pp->id[_RV_insert_check(0,5,71,14,"main",i2)];
  v = pp ->oth[_RV_insert_check(0,6,72,16,"main",i0)][_RV_insert_check(0,7,72,20,"main",i1)];
  v = p.nexts[_RV_insert_check(0,8,73,15,"main",i1)][_RV_insert_check(0,9,73,19,"main",i2)]->nexts[_RV_insert_check(0,8,73,30,"main",i3)][_RV_insert_check(0,9,73,34,"main",*(int *)(_RV_insert_pointer_check(_i_base,_i_bound,73,35,"main",(void *)i)))]->id[_RV_insert_check(0,5,73,42,"main",i4)];
  v = p.q.v;
  v = p.q.id[_RV_insert_check(0,12,75,14,"main",i4)];
  v = p.tq[_RV_insert_check(0,10,76,12,"main",i3)][_RV_insert_check(0,11,76,16,"main",i1)].v;
  v = p.tq[_RV_insert_check(0,10,77,12,"main",i1)][_RV_insert_check(0,11,77,16,"main",i2)].next->tq[_RV_insert_check(0,10,77,29,"main",i3)][_RV_insert_check(0,11,77,33,"main",i4)].v;
  v = pp->tq[_RV_insert_check(0,10,78,14,"main",i3)][_RV_insert_check(0,11,78,18,"main",i1)].v;

  v = p.znexts[i0][i1][i2].val;

  q = p.tq[_RV_insert_check(0,10,82,12,"main",i0)][_RV_insert_check(0,11,82,16,"main",i1)];

  v = p.tq[_RV_insert_check(0,10,84,12,"main",i0)][_RV_insert_check(0,11,84,16,"main",i1)].tr[_RV_insert_check(0,13,84,23,"main",i2)].v;

  v = pp->val;
  v = p.val;
  v = p.tq[_RV_insert_check(0,10,88,12,"main",i0)][_RV_insert_check(0,11,88,16,"main",i1)].v;

  return v;
}
