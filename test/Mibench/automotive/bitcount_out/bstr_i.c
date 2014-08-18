/* +++Date last modified: 05-Jul-1997 */

/*
**  Make an ascii binary string into an integer.
**
**  Public domain by Bob Stout
*/

#include <string.h>
#include "bitops.h"

unsigned int bstr_i(char *cptr)
{
      unsigned int i;
__boundcheck_metadata_store((void *)(&i),(void *)((size_t)(&i)+sizeof(i)*8-1));
unsigned int j = 0;
__boundcheck_metadata_store((void *)(&j),(void *)((size_t)(&j)+sizeof(j)*8-1));


      while (cptr && *(char *)(__boundcheck_ptr_reference(16,23,"bstr_i",(void *)(cptr),(void *)(cptr))) && strchr("01", *(char *)(__boundcheck_ptr_reference(16,45,"bstr_i",(void *)(cptr),(void *)(cptr)))))
      {
            i = *(char *)(__boundcheck_ptr_reference(18,18,"bstr_i",(void *)(cptr),(void *)(cptr++))) - '0';
            j <<= 1;
            j |= (i & 0x01);
      }
      return(j);
}

#ifdef TEST

#include <stdlib.h>

int main(int argc, char *argv[])
{
      char *arg;
      unsigned int x;

      while (--argc)
      {
            x = bstr_i(arg = *++argv);
            printf("Binary %s = %d = %04Xh\n", arg, x, x);
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
