#include <stdlib.h>
#include <stdbool.h>

/*
1) if the object is a simgle variable,store the address of it as the base and bound;
2) if the object is a array,store the begining address and ending address of it;
3) if the object is a pointer,store the base address and bound address of memory,which is the pointer pointe to.
*/
void __boundcheck_metadata_store(void* base, void* bound);

/*cheching is being done when referencing a pointer*/
void* __boundcheck_ptr_reference(int row,int column,void *f_name,void *ptr_org,void *ptr);

size_t __boundcheck_ptr_cast_to_array_reference(int row,int column,void *f_name,void *ptr_org,void *ptr,size_t script);

size_t _RV_insert_check(size_t low,size_t high,size_t row,size_t column,char *f_name,size_t script);

void __boundcheck_metadata_trans_check(void *dest,void *from,void *expr);

/* strcpy is replaced with __boundcheck_strcpy*/
char* __boundcheck_strcpy(char *dest,char *src);

char* __boundcheck_strncpy(char *dest,char *src,int len);

char* __boundcheck_strcat(char *dest,char *src);

char* __boundcheck_strncat(char *dest,char *src,int len);

void* __boundcheck_memcpy(void *dest,void *src,size_t len);

void* __boundcheck_memmove(void *dest,void *src,size_t len);

/*malloc is replaced with __boundcheck_malloc*/ 
void* __boundcheck_malloc(size_t size);

void __boundcheck_free(void *ptr);

void bound_show();
