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
	for(i=0; i<n; i++)
	  { 
	    scanf("%d%d%d",&studs[i].cpp,&studs[i].eng,&studs[i].math);
	  }
}

void average(struct student studs[ ], int n)
{	
	int i;
	for(i=0; i<n; i++)
	   studs[i].aver=(studs[i].cpp+studs[i].eng+studs[i].math)/3;
}

void sort(struct student studs[ ], int n)  //Ñ¡Ôñ·šÅÅÐò
{	
	int i, j, p;
	struct student t;
	for(i=0; i<n-1; i++)
	{ 
           p=i;
	   for(j=i+1; j<n; j++)
           if( studs[j].aver > studs[p].aver ) //±ÈœÏµÄÊÇÔªËØµÄÆœŸùÖµ
              p=j;
           if(p!=i)
	   { 
              t=studs[i]; 
              studs[i]=studs[p]; 
              studs[p]=t; 
           }//œ»»»µÄÊÇÊý×éÔªËØ
       }
}

void output(struct student studs[ ], int n)
{	
	int i;
	for(i=0; i<n; i++)
	{ 
          printf("%s\t%s\t", studs[i].name, studs[i].num);
	  printf("%d\t%d\t",studs[i].cpp,studs[i].eng);
	  printf("%d\t%d\n",studs[i].math,studs[i].aver);
	}
}

int main( )
{	
	struct student studs[4]={ {"Wang","0101"}, {"Li","0102"}, //³õÊŒ»¯
                    {"Cheng","0103"}, {"Liu","0104"} }; //×Ö·ûŽ®
	input(studs, 4);
	average(studs, 4);
	sort(studs, 4);
	output(studs, 4);
        return 1;
}
