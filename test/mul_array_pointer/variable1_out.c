#include"boundcheck.h"
#include<string.h>
#include <stdio.h> 
#include <string.h>
#include <ctype.h>
void filter(char s1[]); 
int palin(char s[]);
int main()
{ 
	char s[80];__boundcheck_metadata_store(&s[0],&s[80-1]);

	scanf("%s",s);
	if(palin(s)) 
		printf("Yes.\n");
	else 
		printf("No.\n");
        return 1;
}
void filter(char s1[]) 
{
	char *p1=s1;
__boundcheck_metadata_store((void *)(&p1),(void *)((size_t)(&p1)+sizeof(p1)*8-1));
char  *p2=s1;
__boundcheck_metadata_store((void *)(&p2),(void *)((size_t)(&p2)+sizeof(p2)*8-1));

	while(*(char *)(__boundcheck_ptr_reference(19,9,"filter",(void *)(p1),(void *)(p1))))
		if(isalpha(*p1))     
		{
			*(char *)(__boundcheck_ptr_reference(22,5,"filter",(void *)(p2),(void *)(p2)))=toupper(*(char *)(__boundcheck_ptr_reference(22,17,"filter",(void *)(p1),(void *)(p1))));  
			p1++; p2++;
		}
		else p1++;
	*(char *)(__boundcheck_ptr_reference(26,3,"filter",(void *)(p2),(void *)(p2)))=0;
	
}

int palin(char s[])
{
	char *head;
__boundcheck_metadata_store((void *)(&head),(void *)((size_t)(&head)+sizeof(head)*8-1));
char  *tail;
__boundcheck_metadata_store((void *)(&tail),(void *)((size_t)(&tail)+sizeof(tail)*8-1));

	char s1[80];__boundcheck_metadata_store(&s1[0],&s1[80-1]);
  
	__boundcheck_strcpy(s1,s);
	filter(s1);
	head=s1;
__boundcheck_metadata_trans_check((void *)(head),(void *)(s1),(void *)(s1));
 
	tail=s1+strlen(s1)-1;
__boundcheck_metadata_trans_check((void *)(tail),(void *)(s1),(void *)(s1 + strlen(s1)));

	while(head<tail)
		if(*(char *)(__boundcheck_ptr_reference(39,7,"palin",(void *)(head),(void *)(head)))==*(char *)(__boundcheck_ptr_reference(39,14,"palin",(void *)(tail),(void *)(tail)))) head++,tail--;
		else return(0);
	return(1);
}
