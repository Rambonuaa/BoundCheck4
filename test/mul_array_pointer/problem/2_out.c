#include"self2.h"
#include<string.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
void  Input(void *base_1,void *up_1,char **p, int n); 
void  Sort(void *base_1,void *up_1,char *p[ ],int n);
void  Print(void *base_1,void *up_1,char **p, int n);
void  Free(void *base_1,void *up_1,char *p[ ], int n);
int main(void)
{ 
	char **p;
void *_p_base=NULL;
void *_p_bound=NULL;

	int n;
	printf("help\n");
	scanf("%d",&n);
	p=(char **)malloc(sizeof(char *[n]));
_p_base=p;
_p_bound=p+sizeof(char *[n])/sizeof(char)-1;
 
	Input(_p_base,_p_bound,p,n);      
	Sort(_p_base,_p_bound,p,n);       //ÅÅÐò
	printf("\n");
	Print(_p_base,_p_bound,p,n);      //Êä³ö
	Free(_p_base,_p_bound,p,n);       //ÊÍ·Å¿ÕŒä
        return 1;
}
void  Input(void *_p_base,void *_p_bound,char **p, int n) // n ×Ö·ûŽ®žöÊý
{ 
	char  tempstr[100]; 
	int i;
	for( i=0 ;i<n;i++)
	{
                scanf("%s",tempstr);	
	    p[_RV_insert_pointertoarray_check(_p_base,_p_bound,29,8,"Input",(void *)(p+i),i)]=(char *)malloc(strlen(tempstr)+1);//·ÖÅäÒ»žöŽ®¿ÕŒä		
		if((strlen(tempstr)+1)<strlen(tempstr)) printf("buffer overflow at 29:8 in Input\n");
		strcpy(p[_RV_insert_pointertoarray_check(_p_base,_p_bound,30,12,"Input",(void *)(p+i),i)],tempstr);
	}
}

void Sort(void *_p_base,void *_p_bound,char *p[ ],int n)/*Ñ¡Ôñ·šÅÅÐò*/
{ 
	char *temp;
void *_temp_base=NULL;
void *_temp_bound=NULL;
 int i,j,k;
	for(i=0;i<n-1;i++)
	{
		k=i;
		for(j=i+1;j<n;j++)
			if( strcmp(p[_RV_insert_pointertoarray_check(_p_base,_p_bound,41,17,"Sort",(void *)(p+k),k)],p[_RV_insert_pointertoarray_check(_p_base,_p_bound,41,22,"Sort",(void *)(p+j),j)])>0 ) k=j;
		if(k!=i)
		{
			temp=p[_RV_insert_pointertoarray_check(_p_base,_p_bound,44,11,"Sort",(void *)(p+i),i)];p[_RV_insert_pointertoarray_check(_p_base,_p_bound,44,16,"Sort",(void *)(p+i),i)]=p[_RV_insert_pointertoarray_check(_p_base,_p_bound,44,21,"Sort",(void *)(p+k),k)];p[_RV_insert_pointertoarray_check(_p_base,_p_bound,44,26,"Sort",(void *)(p+k),k)]=temp;
		}
	} 
}

void  Print(void *_p_base,void *_p_bound,char **p, int n)
{ 
	for(int i=0; i<n; i++)
           printf("%s\n",p[_RV_insert_pointertoarray_check(_p_base,_p_bound,52,28,"Print",(void *)(p+i),i)]);		 
}

void  Free(void *_p_base,void *_p_bound,char *p[ ], int n)
{ 
	for(int i=0; i<n; i++) 
		free(p[_RV_insert_pointertoarray_check(_p_base,_p_bound,58,10,"Free",(void *)(p+i),i)]); 
	free(p);
_p_bound=NULL;

}

