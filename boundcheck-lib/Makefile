all: softboundcets_rt

CFLAGS=-Wall -pedantic  -D__BOUNDCHECK -O3 -fno-strict-aliasing

# If LLVM_GOLD is set, also build a library for use with LTO
#
# Note that the name of the library is hardcoded in the compiler. Thus, we call
# it the same, but put it in the lto/ subdirectory. To use it, pass
# -L/path/to/softboundcets-lib/lto to the compiler, and use -flto during
# compilation and linking

softboundcets_rt: boundcheck.h boundcheck.c tree_api.h avl_tree.c
	clang $(CFLAGS) -c boundcheck.c -o boundcheck.o
	clang $(CFLAGS) -c avl_tree.c -o avl_tree.o
	ar $(ARFLAGS) libboundcheck_rt.a boundcheck.o avl_tree.o



clean:
	rm -rf *.o *.a *~ lto/

