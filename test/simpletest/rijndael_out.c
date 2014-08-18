#include"boundcheck.h"
#include<string.h>
#include<stdio.h>
void fillrand(char *buf, int len)
{
	int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));

	for(i = 0; i < len; ++i)
	{
		(*(char *)(__boundcheck_ptr_reference(7,3,"fillrand",(void *)(&buf[0]),(void *)(&buf[i])))) = len;
	}

}
int main()
{
    char inbuf[16];__boundcheck_metadata_store(&inbuf[0],&inbuf[16-1]);
char var;
__boundcheck_metadata_store((void *)(&var),(void *)((size_t)(&var)+sizeof(var)*8-1));
char  outbuf[16];__boundcheck_metadata_store(&outbuf[0],&outbuf[16-1]);
char  *p[20];__boundcheck_metadata_store(&p[0],&p[20-1]);
char  **q;
__boundcheck_metadata_store((void *)(&q),(void *)((size_t)(&q)+sizeof(q)*8-1));

    char **point;
__boundcheck_metadata_store((void *)(&point),(void *)((size_t)(&point)+sizeof(point)*8-1));

    fillrand(outbuf, 16);
    fillrand(inbuf, 1);
    return 1;
}
	
