#include<stdio.h>
#include<string.h>
int main()
{
	static char tbuf[2][2048];
        char buf[2][3][4];
	char buf1[3][4][5];

	buf[1][1][1]='c';
	strcpy(buf[1][1],"AAA");
	printf("%s",buf[1][1]);

        printf("%d\n",strlen(buf[1][1]));
	memset(tbuf[0],0,2048);
	memset(tbuf[1],0,2048);
	strcpy(tbuf[0],"AAAA");
	strcpy(tbuf[1],tbuf[0]);
	strcat(tbuf[1],"a");
	tbuf[1][2047]='\0';
	return 0;
}
