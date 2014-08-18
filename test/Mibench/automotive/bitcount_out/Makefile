FILE = bitcnt_1.c bitcnt_2.c bitcnt_3.c bitcnt_4.c bitcnts.c bitfiles.c bitstrng.c bstr_i.c 

bitcnts: ${FILE} Makefile
	gcc -static ${FILE} -O3 -o bitcnts -L'/home/administrator/boundcheck4/boundcheck-lib' -lboundcheck_rt -lm

clean:
	rm -rf bitcnts output*
