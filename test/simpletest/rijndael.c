#include<stdio.h>
void fillrand(char *buf, int len)
{
	int i;
	for(i = 0; i < len; ++i)
	{
		buf[i] = len;
	}

}
int main()
{
    char inbuf[16],var,outbuf[16],*p[20],**q;
    char **point;
    fillrand(outbuf, 16);
    fillrand(inbuf, 1);
    return 1;
}
	
