#include<stdio.h>
#include<stdlib.h>
#define nstates0 10
int main()
{
	short *mapstate[5];
	mapstate[0] = (short *) malloc(nstates0 * sizeof(short));
	mapstate[0][1]=0;
	return 0;
}
