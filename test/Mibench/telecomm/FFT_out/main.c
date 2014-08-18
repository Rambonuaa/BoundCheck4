#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
	unsigned MAXSIZE;
__boundcheck_metadata_store(&(MAXSIZE),&(MAXSIZE));

	unsigned MAXWAVES;
__boundcheck_metadata_store(&(MAXWAVES),&(MAXWAVES));

	unsigned i;
__boundcheck_metadata_store(&(i),&(i));
unsigned int j;
__boundcheck_metadata_store(&(j),&(j));

	float *RealIn;
__boundcheck_metadata_store((void *)(RealIn),(void *)(RealIn));

	float *ImagIn;
__boundcheck_metadata_store((void *)(ImagIn),(void *)(ImagIn));

	float *RealOut;
__boundcheck_metadata_store((void *)(RealOut),(void *)(RealOut));

	float *ImagOut;
__boundcheck_metadata_store((void *)(ImagOut),(void *)(ImagOut));

	float *coeff;
__boundcheck_metadata_store((void *)(coeff),(void *)(coeff));

	float *amp;
__boundcheck_metadata_store((void *)(amp),(void *)(amp));

	int invfft=0;
__boundcheck_metadata_store(&(invfft),&(invfft));


	if (argc<3)
	{
		printf("Usage: fft <waves> <length> -i\n");
		printf("-i performs an inverse fft\n");
		printf("make <waves> random sinusoids");
		printf("<length> is the number of samples\n");
		exit(-1);
	}
	else if (argc==4)
		invfft = !strncmp(argv[3],"-i",2);
	MAXSIZE=atoi(argv[2]);
	MAXWAVES=atoi(argv[1]);
		
 srand(1);

 RealIn=(float*)__boundcheck_malloc(sizeof(float)*MAXSIZE);
 ImagIn=(float*)__boundcheck_malloc(sizeof(float)*MAXSIZE);
 RealOut=(float*)__boundcheck_malloc(sizeof(float)*MAXSIZE);
 ImagOut=(float*)__boundcheck_malloc(sizeof(float)*MAXSIZE);
 coeff=(float*)__boundcheck_malloc(sizeof(float)*MAXWAVES);
 amp=(float*)__boundcheck_malloc(sizeof(float)*MAXWAVES);

 /* Makes MAXWAVES waves of random amplitude and period */
	for(i=0;i<MAXWAVES;i++) 
	{
		coeff[__boundcheck_ptr_cast_to_array_reference(42,9,"main",(void *)(coeff),(void *)(coeff+i),i)] = rand()%1000;
		amp[__boundcheck_ptr_cast_to_array_reference(43,7,"main",(void *)(amp),(void *)(amp+i),i)] = rand()%1000;
	}
 for(i=0;i<MAXSIZE;i++) 
 {
   /*   RealIn[i]=rand();*/
	 RealIn[__boundcheck_ptr_cast_to_array_reference(48,10,"main",(void *)(RealIn),(void *)(RealIn+i),i)]=0;
	 for(j=0;j<MAXWAVES;j++) 
	 {
		 /* randomly select sin or cos */
		 if (rand()%2)
		 {
		 		RealIn[__boundcheck_ptr_cast_to_array_reference(54,13,"main",(void *)(RealIn),(void *)(RealIn+i),i)]+=coeff[__boundcheck_ptr_cast_to_array_reference(54,23,"main",(void *)(coeff),(void *)(coeff+j),j)]*cos(amp[__boundcheck_ptr_cast_to_array_reference(54,34,"main",(void *)(amp),(void *)(amp+j),j)]*i);
			}
		 else
		 {
		 	RealIn[__boundcheck_ptr_cast_to_array_reference(58,12,"main",(void *)(RealIn),(void *)(RealIn+i),i)]+=coeff[__boundcheck_ptr_cast_to_array_reference(58,22,"main",(void *)(coeff),(void *)(coeff+j),j)]*sin(amp[__boundcheck_ptr_cast_to_array_reference(58,33,"main",(void *)(amp),(void *)(amp+j),j)]*i);
		 }
  	 ImagIn[__boundcheck_ptr_cast_to_array_reference(60,12,"main",(void *)(ImagIn),(void *)(ImagIn+i),i)]=0;
	 }
 }

 /* regular*/
 fft_float (MAXSIZE,invfft,RealIn,ImagIn,RealOut,ImagOut);
 
 printf("RealOut:\n");
 for (i=0;i<MAXSIZE;i++)
   printf("%f \t", RealOut[__boundcheck_ptr_cast_to_array_reference(69,28,"main",(void *)(RealOut),(void *)(RealOut+i),i)]);
 printf("\n");

printf("ImagOut:\n");
 for (i=0;i<MAXSIZE;i++)
   printf("%f \t", ImagOut[__boundcheck_ptr_cast_to_array_reference(74,28,"main",(void *)(ImagOut),(void *)(ImagOut+i),i)]);
   printf("\n");

 __boundcheck_free(RealIn);
 __boundcheck_free(ImagIn);
 __boundcheck_free(RealOut);
 __boundcheck_free(ImagOut);
 __boundcheck_free(coeff);
 __boundcheck_free(amp);
 exit(0);


}
