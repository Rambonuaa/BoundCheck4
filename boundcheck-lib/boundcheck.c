#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include"tree_api.h"
#include"boundcheck.h"
void __boundcheck_metadata_store(void* base, void* bound) 
{
//   printf("object store base=%p  bound=%p length=%d\n",base,bound,(size_t)bound-(size_t)base);
   __metadata_unit* new_metadata=(__metadata_unit *)malloc(sizeof(__metadata_unit));
   if(new_metadata!=NULL)
   {
      new_metadata->base=(size_t)base;
      new_metadata->bound=(size_t)bound;
   }
   __add_element(new_metadata);
}

void __boundcheck_metadata_trans_check(void *dest,void *from,void *expr)
{
   
   __metadata_unit *tmp_from=__get_cont(from);
   if(tmp_from==NULL)
   {
       printf("Memory Block Is Not Found!\n");
       exit(1);
   }
   
   __metadata_unit *tmp_expr=__get_cont(expr);
   if(tmp_expr==NULL||tmp_expr->base!=tmp_from->base)
   {
       printf("Pointer adding is out of the border!");
       exit(1);
   }

   __metadata_unit *tmp_dest=__get_cont(dest);
   if((tmp_dest==NULL)&&(tmp_dest->base==tmp_from->base))
       __boundcheck_metadata_store((void *)(tmp_from->base),(void *)(tmp_from->bound));
   
   if(tmp_dest!=NULL&&tmp_dest!=NULL&&tmp_dest->base==tmp_from->base)
   {
      tmp_dest->base=tmp_from->base;
      tmp_dest->bound=tmp_from->bound;
   }
}



void* __boundcheck_ptr_reference(int row,int column,void *f_name,void *ptr_org,void *ptr)
{
//    printf("reference ptr=%p ||base ptr=%p\n",ptr,ptr_org);
    __metadata_unit *tmp_org=__get_cont(ptr_org);
    __metadata_unit *tmp_ptr=__get_cont(ptr);
    if(tmp_ptr==NULL||tmp_org==NULL)
    {
       printf("Memory Block Is Not Found in %s:%d:%d!\n",f_name,row,column);
       exit(1);
    }
//    printf("pointer read1 ptr_base=%p  ptr_bound=%p length=%d\nptr_org_base=%p  ptr_org_bound=%p length=%d\n",(void *)(tmp_ptr->base),(void *)(tmp_ptr->bound),tmp_ptr->bound-tmp_ptr->base,(void *)(tmp_org->base),(void *)(tmp_org->bound),tmp_org->bound-tmp_org->base);
    if(tmp_org->base!=tmp_ptr->base)
    {
       printf("Boundchecking is triggered in %s,at %d:%d(ptrs is Not in the same Memory Block!)\n",f_name,row,column);
       exit(1);
    }
//    printf("pointer read2 base=%p  bound=%p length=%d\n",(void *)(tmp_ptr->base),(void *)(tmp_ptr->bound),tmp_ptr->bound-tmp_ptr->base);
    if((size_t)ptr<tmp_ptr->base||(size_t)ptr>tmp_ptr->bound)
    {
       printf("Boundchecking is triggered in %s,at %d:%d(ptr is illegal!)\n",f_name,row,column);
       exit(1);
    }
    return ptr;
}

size_t __boundcheck_ptr_cast_to_array_reference(int row,int column,void *f_name,void *ptr_org,void *ptr,size_t script)
{
//    printf("reference ptr=%p ||base ptr=%p\n",ptr,ptr_org);
    __metadata_unit *tmp_org=__get_cont(ptr_org);
    __metadata_unit *tmp_ptr=__get_cont(ptr);
    if(tmp_ptr==NULL||tmp_org==NULL)
    {
       printf("Memory Block Is Not Found in %s:%d:%d!\n",f_name,row,column);
       exit(1);
    }
//    printf("cast read1 base=%p  bound=%p length=%d\n",(void *)(tmp_ptr->base),(void *)(tmp_ptr->bound),tmp_ptr->bound-tmp_ptr->base);
    if(tmp_org->base!=tmp_ptr->base)
    {
       printf("Boundchecking is triggered in %s,at %d:%d(ptrs is Not in the same Memory Block!)\n",f_name,row,column);
       exit(1);
    }
//    printf("cast read2 base=%p  bound=%p length=%d\n",(void *)(tmp_ptr->base),(void *)(tmp_ptr->bound),tmp_ptr->bound-tmp_ptr->base);
    if((size_t)ptr<tmp_ptr->base||(size_t)ptr>tmp_ptr->bound)
    {
       printf("Boundchecking is triggered in %s,at %d:%d(ptr is illegal!)\n",f_name,row,column);
       exit(1);
    }
    return script;
}

size_t _RV_insert_check(size_t low,size_t high,size_t row,size_t column,char *f_name,size_t script)
{
      if(script>high||script<low)
      {
          FILE *fp=fopen("error.txt","at");
          printf("%d:%d array bound violations in %s\n",row,column,f_name);
          fprintf(fp,"%d:%d array bound violations in %s\n",row,column,f_name);
          fclose(fp);
          exit(-1);
      }
      return script;
}


char* __boundcheck_strcpy(char *dest,char *src)
{
//   printf("dest=%p\n",dest);
   __metadata_unit *tmp_dest=__get_cont(dest);
   size_t size=strlen(src);
   if(tmp_dest==NULL)
   {
        printf("Not found in Tree in strcpy!\n");
        exit(1);
   }
   if((tmp_dest->bound-tmp_dest->base-1)<size)
   {
       printf("the length in dest is Not enough in strcpy!\n");
       exit(1);
   }
   char *ret_ptr=strcpy(dest,src);
   return ret_ptr;
} 

char* __boundcheck_strncpy(char *dest,char *src,int len)
{
   __metadata_unit *tmp_dest=__get_cont(dest);
   __metadata_unit *tmp_src=__get_cont(src);
   if(tmp_dest==NULL)
   {
        printf("Not found in Tree in strncpy!\n");
        exit(1);
   }
   if(((tmp_dest->bound-tmp_dest->base)<len)||((tmp_src->bound-tmp_src->base)<len))
   {
       printf("the length in dest is Not enough in strncpy!\n");
       exit(1);
   }
   char *ret_ptr=strncpy(dest,src,len);
   return ret_ptr;
} 

char* __boundcheck_strcat(char *dest,char *src)
{	
	__metadata_unit *tmp_dest=__get_cont(dest);
	if(tmp_dest==NULL)
	{
		printf("Not Found In Tree!Operate:strcat\n");
		exit(1);
	}
	if((tmp_dest->bound-tmp_dest->base-1)<(strlen(dest)+strlen(src)))
	{
		printf("dest add src is larger than dest in strcat!\n");
		exit(1);
	}
	char *ret_ptr=strcat(dest,src);
	return ret_ptr;
}

char* __boundcheck_strncat(char *dest,char *src,int len)
{
	__metadata_unit *tmp_dest=__get_cont(dest);
	__metadata_unit *tmp_src=__get_cont(src);
	if(tmp_dest==NULL)
	{
		printf("Not Found In Tree!Operate:strcat\n");
		exit(1);
	}
	if(((tmp_dest->bound-tmp_dest->base-1)<(strlen(dest)+len))||((tmp_src->bound-tmp_src->base)<len))
	{
		printf("dest add src is larger than dest in strncat!\n");
		exit(1);
	}
	char *ret_ptr=strncat(dest,src,len);
	return ret_ptr;
}


void* __boundcheck_memcpy(void *dest,void *src,size_t len)
{
	__metadata_unit *tmp_dest=__get_cont(dest);
	__metadata_unit *tmp_src=__get_cont(src);
	if(tmp_dest==NULL||tmp_src==NULL)
	{
		printf("Not Found In Tree!Operate:strcat\n");
		exit(1);
	}
	if(((tmp_dest->bound-(size_t)dest)<len)||((tmp_src->bound-(size_t)src)<len))
	{
		printf("dest plus src is larger than dest!\n");
		exit(1);
	}
	void *ret_ptr=memcpy(dest,src,len);
	return ret_ptr;
}

void* __boundcheck_memmove(void *dest,void *src,size_t len)
{
	__metadata_unit *tmp_dest=__get_cont(dest);
	__metadata_unit *tmp_src=__get_cont(src);
	if(tmp_dest==NULL||tmp_src==NULL)
	{
		printf("Not Found In Tree!Operate:strcat\n");
		exit(1);
	}
	if(((tmp_dest->bound-(size_t)dest)<len)||((tmp_src->bound-(size_t)src)<len))
	{
		printf("dest plus src is larger than dest!\n");
		exit(1);
	}
	void *ret_ptr=memmove(dest,src,len);
	return ret_ptr;
}

void* __boundcheck_malloc(size_t size)
{
   
   char *tmp=(char *)malloc(size);
   if(tmp==NULL)
   {
       printf("malloc1 is failed!\n");
       return NULL;
   }
   __metadata_unit* new_metadata=(__metadata_unit *)malloc(sizeof(__metadata_unit));
   if(new_metadata==NULL)
   {
       printf("malloc2 is failed!\n");
       return NULL;
   }
   new_metadata->base=(size_t)tmp;
   new_metadata->bound=(size_t)(tmp+size);
//   printf("malloc store base=%p  bound=%p length=%d\n",(void *)new_metadata->base,(void *)new_metadata->bound,new_metadata->bound-new_metadata->base);
   __add_element(new_metadata);
   return tmp;
}

void __boundcheck_free(void *ptr)
{
	if(ptr==NULL)
	{
		printf("Ptr is NULL\n");
		exit(1);
	}
	__metadata_unit *tmp=__get_exact(ptr);
	if(tmp==NULL)
	{
		printf("Not exit or doubel free!\n");
		exit(1);
	}
	__remove_element(tmp);
	free(ptr);
}

void bound_show()
{
//	show();
}
