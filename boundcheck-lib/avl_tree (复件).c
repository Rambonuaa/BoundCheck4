/*
首先平衡二叉树是一个二叉排序树；
其基本思想是：
在构建二叉排序树的过程中，当每插入一个节点时，
先检查是否因为插入而破坏了树的平衡性，若是，
找出最小不平衡树，进行适应的旋转，使之成为新的平衡二叉树。
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"tree_api.h"
#include "boundcheck.h"
#define LH 1
#define EH 0
#define RH -1 
//int deep=0; 
 
typedef struct BTNode
{
	__metadata_unit * value;
	int BF;//平衡因子（balance factor）
	struct BTNode *lchild,*rchild;
}BTNode,*BTree; 

BTree __root=NULL;

void R_Rotate(BTree *p)//以p为根节点的二叉排序树进行右旋转
{
	BTree L;
	L=(*p)->lchild;
	(*p)->lchild=L->rchild;
	L->rchild=(*p);
	*p=L;//p指向新的根节点
}
 
void L_Rotate(BTree *p)//以p为根节点的二叉排序树进行左旋转
{
	BTree R;
	R=(*p)->rchild;
	(*p)->rchild=R->lchild;
	R->lchild=(*p);
	*p=R;
}
 
void LeftBalance(BTree *T)
{
	BTree L,Lr;
	L=(*T)->lchild;
	switch(L->BF)
	{
		//检查T的左子树平衡度，并作相应的平衡处理
		case LH://新节点插入在T的左孩子的左子树上，做单右旋处理
			(*T)->BF=L->BF=EH;
			R_Rotate(T);
			break;
		case RH://新插入节点在T的左孩子的右子树上，做双旋处理
			Lr=L->rchild;
			switch(Lr->BF)
			{
				case LH:
					(*T)->BF=RH;
					L->BF=EH;
					break;
				case EH:
					(*T)->BF=L->BF=EH;
					break;
				case RH:
					(*T)->BF=EH;
					L->BF=LH;
					break;
			}
		Lr->BF=EH;
		L_Rotate(&(*T)->lchild);
		R_Rotate(T);
	}
} 
void RightBalance(BTree *T)
{
	BTree R,Rl;
	R=(*T)->rchild;
	switch(R->BF)
	{
		case RH://新节点插在T的右孩子的右子树上，要做单左旋处理
			(*T)->BF=R->BF=EH;
			L_Rotate(T);
			break;
		case LH://新节点插在T的右孩子的左子树上，要做双旋处理
			Rl=R->lchild;
			switch(Rl->BF)
			{
				case LH:
					(*T)->BF=EH;
					R->BF=RH;
					break;
				case EH:
					(*T)->BF=R->BF=EH;
					break;
				case RH:
					(*T)->BF=LH;
					R->BF=EH;
					break;
			}
		Rl->BF=EH;
		R_Rotate(&(*T)->rchild);
		L_Rotate(T);
	}
}
 
bool InsertAVL(BTree *T,__metadata_unit *e,bool *taller)//变量taller反应T长高与否
{
	if(!*T)
	{
		*T=(BTree)malloc(sizeof(BTNode));
		(*T)->value=e;
		(*T)->lchild=(*T)->rchild=NULL;
		(*T)->BF=EH;
		*taller=true;
	}
	else
	{
		if(e->base==(*T)->value->base)//不插入
		{
			*taller=false;
			return false; 
		}
		if(e->base<(*T)->value->base)
		{
			if(!InsertAVL(&(*T)->lchild,e,taller))//未插入
				return false;
			if(*taller)//以插入左子树，且左子树变高
			{
				switch((*T)->BF)
				{
					case LH://原本左子树比右子树高，需要做左平衡处理
						LeftBalance(T);
						*taller=false;
						break;
					case EH://原本左右子树等高，现因左子树增高而树增高
						(*T)->BF=LH;
						*taller=true;
						break;
					case RH://原本右子树比左子树高，现在左右子树等高
						(*T)->BF=EH;
						*taller=false;
						break;
				}
			}
		}
		else
		{
			//应在T的右子树中搜寻
			if(!InsertAVL(&(*T)->rchild,e,taller))
				return false;
			if(*taller)//插入右子树，且右子树长高
			{
				switch((*T)->BF)
				{
					case LH://原本左子树比右子树高，现在左右子树等高
						(*T)->BF=EH;
						*taller=false;
						break;
					case EH://原本左右子树等高，现在右子树变高
						(*T)->BF=RH;
						*taller=true;
						break;
					case RH://原本右子树比左子树高，现在需做右平衡处理
						RightBalance(T);
						*taller=false;
						break;
				}
			}
		}
	}
	return true;
}

void __add_element(__metadata_unit *e)
{
	bool taller;
	InsertAVL(&__root,e,&taller);
}

void __remove_element(__metadata_unit* ptr)
{
//现在用不到这个函数
}

__metadata_unit* __get_exact(void *ptr)
{
	BTNode * tmp = __root;
	while(tmp != NULL)
	{
		if(tmp->value->base > (size_t)ptr) tmp = tmp->lchild;
		else if(tmp->value->base < (size_t)ptr) tmp = tmp->rchild;
		else return tmp->value;
	}
	return NULL;
}

__metadata_unit* _get_cont_rec(BTNode * node, void* ptr)
{
//	deep++;
	
	__metadata_unit *tmp1=NULL;
	__metadata_unit *tmp2=NULL;
	if(node == NULL)
	{ 
//		printf("deep is %d\n",deep);	
		return NULL;
	}
	tmp1=__get_exact(ptr);
	if(tmp1!=NULL)
		return tmp1;
	
	BTNode * tmp= node;
	 while(tmp != NULL)
	{
		printf("FFFFFFFFFF2\n");
		if(tmp->value->base > (size_t)ptr) tmp = tmp->lchild;
		else if(tmp->value->bound >= (size_t)ptr) {tmp1 = tmp->value;break;}
		else tmp=tmp->rchild;
	}

	
	if(tmp1!=NULL)
	{
		tmp= node;
		 while(tmp != NULL)
		{
			printf("FFFFFFFFFF2\n");
			if(tmp->value->base > (size_t)ptr) tmp = tmp->lchild;
			else if(tmp->value->bound >= (size_t)ptr) 
				{
					if(tmp1==tmp->value)
						tmp=tmp->rchild;
					else {tmp2 = tmp->value;break;}
				}
			else tmp=tmp->rchild;
		}
	}
	printf("tmp1 = %p tmp2 = %p\n",(void *)(tmp1->base),(void *)(tmp2->base));
	if(tmp1!=NULL&&tmp2==NULL)
		return tmp1;
	if(tmp1!=NULL&&tmp2!=NULL)
		return tmp2;
	return NULL;
/*
	if(node->value->base > (size_t)ptr) return _get_cont_rec(node->lchild, ptr);
	else if((node->value->base <= (size_t)ptr)&&(node->value->bound >= (size_t)ptr))
	{
//		printf("deep is %d\n",deep);
		return node->value;
	}
	else return _get_cont_rec(node->rchild, ptr);
*/
}


__metadata_unit* __get_cont(void* ptr)
{
//	deep=0;
	return _get_cont_rec(__root, ptr);
}

/*
bool Find(BTree T,int p)
{
	if(!T)
		return false;
	else if(T->value->base==p)
		return true;
	else if(T->value->base<p)
		return Find(T->rchild,p);
	else
		return Find(T->lchild,p);
}
 
void Output(BTree T)
{
	if(T)
	{
		printf("%p",(void *)T->value->base);
		if(T->lchild||T->rchild)
		{
			printf("(");
			Output(T->lchild);
			printf(",");
			Output(T->rchild);
			printf(")");
		}
	}
}
 
int main(int argc,char *argv[])
{
	int i=0;
	__metadata_unit* A[9];
	__metadata_unit *result;
	
	double look=0x0ce8;
	while(i<9)
	{
		__metadata_unit *new=(__metadata_unit *)malloc(sizeof(__metadata_unit));
		if(new!=NULL)
		{
			new->base=0x0ce8+i*20;
			new->bound=new->base+20;
		}
		A[i]=new;
		i++;
	}
	printf("AAAAAAAAAAAAAA\n");
	
	for(i=0;i<9;i++)
		__add_element(A[i]);

	printf("DDDDDDDDDDDD\n");
	Output(__root);
	printf("\n");
	
	result=__get_exact(&look);
	if(result)
//	if(Find(__root,look))
		printf("6 is find in the AVL tree!\n");
	else
		printf("6 is not find in the AVL tree!\n");
	return 0;
}
*/ 
