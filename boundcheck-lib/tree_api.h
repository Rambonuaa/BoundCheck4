/**************************************************************************/
//boundcheck tree interface(API)
/**************************************************************************/

#include <stdlib.h>
#include <stdbool.h>

/* Memory block allocated and may be deallocated */
typedef struct {
  size_t base;    //begin address
  size_t bound;   //end address
} __metadata_unit;

/*1 remove the block from the structure */
void  __remove_element(__metadata_unit *);

/*1 add a block in the structure */
void  __add_element(__metadata_unit *);

/*1 return the block B such as : begin addr of B == ptr
   we suppose that such a block exists, but we could return NULL if not */
__metadata_unit * __get_exact(void *);


/*1 return the block B containing ptr, such as :
   begin addr of B <= ptr < (begin addr + size) of B
   or NULL if such a block does not exist */
__metadata_unit * __get_cont(void *);

//void show();


