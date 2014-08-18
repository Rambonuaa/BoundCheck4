FILE1 = bmhasrch.c  bmhisrch.c  bmhsrch.c  pbmsrch_small.c 
FILE2 = bmhasrch.c  bmhisrch.c  bmhsrch.c  pbmsrch_large.c 

all: search_small search_large

search_small: ${FILE1} Makefile
	gcc -static ${FILE1} -O3 -o search_small -L'/home/administrator/boundcheck4/boundcheck-lib' -lboundcheck_rt -lm

search_large: ${FILE2} Makefile
	gcc -static ${FILE2} -O3 -o search_large -L'/home/administrator/boundcheck4/boundcheck-lib' -lboundcheck_rt -lm

clean:
	rm -rf search_small search_large output*
