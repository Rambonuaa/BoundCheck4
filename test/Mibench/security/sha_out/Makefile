# By default, the code is compiled for a "big endian" machine.
# To compile on a "little endian" machine set the LITTLE_ENDIAN flag.
# To make smaller object code, but run a little slower, don't use UNROLL_LOOPS.
# To use NIST's modified SHA of 7/11/94, define USE_MODIFIED_SHA
OBJ=sha_driver.o sha.o
CC = gcc
LIBDIR=-L'/home/administrator/boundcheck4/boundcheck-lib' -lboundcheck_rt
CFLAGS = -O3 -Wall

sha:	${OBJ} Makefile
	$(CC) -o $@ ${OBJ} ${LIBDIR} -lm
sha_driver.o:sha_driver.c
	gcc -c sha_driver.c
sha.o:sha.c
	gcc -c sha.c

clean:
	rm -rf *.o sha output*
