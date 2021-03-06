#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNLIMIT
#define MAXARRAY 60000 /* this number, if too large, will cause a seg. fault!! */

struct myStringStruct {
  char qstring[128];
};

int compare(const void *elem1, const void *elem2)
{
  int result;
__boundcheck_metadata_store((void *)(&result),(void *)((size_t)(&result)+sizeof(result)*8-1));

  
  result = strcmp((*(struct myStringStruct *)(__boundcheck_ptr_reference(16,21,"compare",(void *)((struct myStringStruct *)elem1),(void *)((struct myStringStruct *)elem1)))).qstring, (*(struct myStringStruct *)(__boundcheck_ptr_reference(16,66,"compare",(void *)((struct myStringStruct *)elem2),(void *)((struct myStringStruct *)elem2)))).qstring);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


int
main(int argc, char *argv[]) {
  struct myStringStruct array[MAXARRAY];
__boundcheck_metadata_store((void *)(&array),(void *)((size_t)(&array)+sizeof(array)*8-1));

  FILE *fp;
__boundcheck_metadata_store((void *)(&fp),(void *)((size_t)(&fp)+sizeof(fp)*8-1));

  int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int count=0;
__boundcheck_metadata_store((void *)(&count),(void *)((size_t)(&count)+sizeof(count)*8-1));

  
  if (argc<2) {
    fprintf(stderr,"Usage: qsort_small <file>\n");
    exit(-1);
  }
  else {
    fp = fopen(argv[1],"r");
    
    while((fscanf(fp, "%s", &array[_RV_insert_check(0,60000,35,36,"main",count)].qstring) == 1) && (count < MAXARRAY)) {
	 count++;
    }
  }
  printf("\nSorting %d elements.\n\n",count);
  qsort(array,count,sizeof(struct myStringStruct),compare);
  
  for(i=0;i<count;i++)
    printf("%s\n", array[_RV_insert_check(0,60000,43,26,"main",i)].qstring);
  return 0;
}
