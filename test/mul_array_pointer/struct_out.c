#include"boundcheck.h"
#include<string.h>
#include  <stdio.h>
struct student
{
	char name[20];    
	char num[10];     
	int cpp,eng,math;  
	int aver;             
};

void input(struct student studs[ ], int n)
{	
	int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

	for(i=0; i<n; i++)
	  { 
	    scanf("%d%d%d",&(*(struct student *)(__boundcheck_ptr_reference(15,22,"input",(void *)(&studs[0]),(void *)(&studs[i])))).cpp,&(*(struct student *)(__boundcheck_ptr_reference(15,36,"input",(void *)(&studs[0]),(void *)(&studs[i])))).eng,&(*(struct student *)(__boundcheck_ptr_reference(15,50,"input",(void *)(&studs[0]),(void *)(&studs[i])))).math);
	  }
}

void average(struct student studs[ ], int n)
{	
	int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

	for(i=0; i<n; i++)
	   (*(struct student *)(__boundcheck_ptr_reference(23,5,"average",(void *)(&studs[0]),(void *)(&studs[i])))).aver=((*(struct student *)(__boundcheck_ptr_reference(23,20,"average",(void *)(&studs[0]),(void *)(&studs[i])))).cpp+(*(struct student *)(__boundcheck_ptr_reference(23,33,"average",(void *)(&studs[0]),(void *)(&studs[i])))).eng+(*(struct student *)(__boundcheck_ptr_reference(23,46,"average",(void *)(&studs[0]),(void *)(&studs[i])))).math)/3;
}

void sort(struct student studs[ ], int n)  //Ñ¡Ôñ·šÅÅÐò
{	
	int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
int  j;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));
int  p;
__boundcheck_metadata_store((void *)(&p),(void *)((size_t)(&p)+sizeof(p)*8-1));

	struct student t;
__boundcheck_metadata_store((void *)(&t),(void *)((size_t)(&t)+sizeof(t)*8-1));

	for(i=0; i<n-1; i++)
	{ 
           p=i;
	   for(j=i+1; j<n; j++)
           if( (*(struct student *)(__boundcheck_ptr_reference(34,16,"sort",(void *)(&studs[0]),(void *)(&studs[j])))).aver > (*(struct student *)(__boundcheck_ptr_reference(34,32,"sort",(void *)(&studs[0]),(void *)(&studs[p])))).aver ) //±ÈœÏµÄÊÇÔªËØµÄÆœŸùÖµ
              p=j;
           if(p!=i)
	   { 
              t=(*(struct student *)(__boundcheck_ptr_reference(38,17,"sort",(void *)(&studs[0]),(void *)(&studs[i])))); 
              (*(struct student *)(__boundcheck_ptr_reference(39,15,"sort",(void *)(&studs[0]),(void *)(&studs[i]))))=(*(struct student *)(__boundcheck_ptr_reference(39,24,"sort",(void *)(&studs[0]),(void *)(&studs[p])))); 
              (*(struct student *)(__boundcheck_ptr_reference(40,15,"sort",(void *)(&studs[0]),(void *)(&studs[p]))))=t; 
           }//œ»»»µÄÊÇÊý×éÔªËØ
       }
}

void output(struct student studs[ ], int n)
{	
	int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

	for(i=0; i<n; i++)
	{ 
          printf("%s\t%s\t", (*(struct student *)(__boundcheck_ptr_reference(50,30,"output",(void *)(&studs[0]),(void *)(&studs[i])))).name, (*(struct student *)(__boundcheck_ptr_reference(50,45,"output",(void *)(&studs[0]),(void *)(&studs[i])))).num);
	  printf("%d\t%d\t",(*(struct student *)(__boundcheck_ptr_reference(51,22,"output",(void *)(&studs[0]),(void *)(&studs[i])))).cpp,(*(struct student *)(__boundcheck_ptr_reference(51,35,"output",(void *)(&studs[0]),(void *)(&studs[i])))).eng);
	  printf("%d\t%d\n",(*(struct student *)(__boundcheck_ptr_reference(52,22,"output",(void *)(&studs[0]),(void *)(&studs[i])))).math,(*(struct student *)(__boundcheck_ptr_reference(52,36,"output",(void *)(&studs[0]),(void *)(&studs[i])))).aver);
	}
}

int main( )
{	
	struct student studs[4]={ {"Wang","0101"}, {"Li","0102"}, //³õÊŒ»¯
                    {"Cheng","0103"}, {"Liu","0104"} };__boundcheck_metadata_store(&studs[0],&studs[4-1]);
 //×Ö·ûŽ®
	input(studs, 4);
	average(studs, 4);
	sort(studs, 4);
	output(studs, 4);
        return 1;
}
