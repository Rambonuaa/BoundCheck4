#ifndef _SELF_H_
#define _SELF_H_
#include<stdio.h>
#include<stdlib.h>
int _RV_insert_check(int low,int high,int row,int column,char *f_name,int script)
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

int _RV_insert_pointertoarray_check(void *low,void *high,int row,int column,char *f_name,void *pointer_name,int script)
{
      if(pointer_name>high||pointer_name<low)
      {
          FILE *fp=fopen("error.txt","at");
          printf("%d:%d pointer to array buffer overflow in %s\n",row,column,f_name);
          fprintf(fp,"%d:%d pointer to array buffer overflow in %s\n",row,column,f_name);
          fclose(fp);
          exit(-1);
      }
      return script;
}

void* _RV_insert_pointer_check(void *low,void *high,int row,int column,char *f_name,void *script)
{
      int isoverflow=1;
      if(high==NULL)
      {
          printf("%d:%d uninitialized %s\n",row,column,f_name);
          exit(-1);
      }
      if(script<low)
          isoverflow=0;
      if(script>high||script<low)
      {
          FILE *fp=fopen("error.txt","at");
          if(isoverflow)
          {
             printf("%d:%d Pointer overflow in %s\n",row,column,f_name);
             fprintf(fp,"%d:%d Pointer overflow in %s\n",row,column,f_name);
          }
          else
          {
             printf("%d:%d Pointer underflow in %s\n",row,column,f_name);
             fprintf(fp,"%d:%d Pointer underflow in %s\n",row,column,f_name);
          }
          fclose(fp);
          exit(-1);
      }
      return script;
}
#endif
   
  

