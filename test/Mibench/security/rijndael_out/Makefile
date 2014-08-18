rijndael: aes.c aes.h aestab.h aesxam.c Makefile
	gcc -static -O3 -o rijndael aes.c aesxam.c -L'/home/administrator/boundcheck4/boundcheck-lib' -lboundcheck_rt -lm

clean:
	rm -rf rijndael output*
