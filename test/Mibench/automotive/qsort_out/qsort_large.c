#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define UNLIMIT
#define MAXARRAY 60000 /* this number, if too large, will cause a seg. fault!! */

struct my3DVertexStruct {
  int x, y, z;
  double distance;
};

int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1;
__boundcheck_metadata_store((void *)(&distance1),(void *)((size_t)(&distance1)+sizeof(distance1)*8-1));
double distance2;
__boundcheck_metadata_store((void *)(&distance2),(void *)((size_t)(&distance2)+sizeof(distance2)*8-1));


  distance1 = (*(struct my3DVertexStruct *)(__boundcheck_ptr_reference(20,17,"compare",(void *)((struct my3DVertexStruct *)elem1),(void *)((struct my3DVertexStruct *)elem1)))).distance;
  distance2 = (*(struct my3DVertexStruct *)(__boundcheck_ptr_reference(21,17,"compare",(void *)((struct my3DVertexStruct *)elem2),(void *)((struct my3DVertexStruct *)elem2)))).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}


int
main(int argc, char *argv[]) {
  struct my3DVertexStruct array[MAXARRAY];
__boundcheck_metadata_store((void *)(&array),(void *)((size_t)(&array)+sizeof(array)*8-1));

  FILE *fp;
__boundcheck_metadata_store((void *)(&fp),(void *)((size_t)(&fp)+sizeof(fp)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int count=0;
__boundcheck_metadata_store((void *)(&count),(void *)((size_t)(&count)+sizeof(count)*8-1));

  int x;
__boundcheck_metadata_store((void *)(&x),(void *)((size_t)(&x)+sizeof(x)*8-1));
int y;
__boundcheck_metadata_store((void *)(&y),(void *)((size_t)(&y)+sizeof(y)*8-1));
int z;
__boundcheck_metadata_store((void *)(&z),(void *)((size_t)(&z)+sizeof(z)*8-1));

  
  if (argc<2) {
    fprintf(stderr,"Usage: qsort_large <file>\n");
    exit(-1);
  }
  else {
    fp = fopen(argv[1],"r");
    
    while((fscanf(fp, "%d", &x) == 1) && (fscanf(fp, "%d", &y) == 1) && (fscanf(fp, "%d", &z) == 1) &&  (count < MAXARRAY)) {
	 array[_RV_insert_check(0,60000,42,9,"main",count)].x = x;
	 array[_RV_insert_check(0,60000,43,9,"main",count)].y = y;
	 array[_RV_insert_check(0,60000,44,9,"main",count)].z = z;
	 array[_RV_insert_check(0,60000,45,9,"main",count)].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	 count++;
    }
  }
  printf("\nSorting %d vectors based on distance from the origin.\n\n",count);
  qsort(array,count,sizeof(struct my3DVertexStruct),compare);
  
  for(i=0;i<count;i++)
    printf("%d %d %d\n", array[_RV_insert_check(0,60000,53,32,"main",i)].x, array[_RV_insert_check(0,60000,53,44,"main",i)].y, array[_RV_insert_check(0,60000,53,56,"main",i)].z);
  return 0;
}
