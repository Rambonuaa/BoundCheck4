
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
#define LH 1
#define EH 0
#define RH -1  
 
typedef struct BTNode
{
	int data;
	int BF;//平衡因子（balance factor）
	struct BTNode *lchild,*rchild;
}BTNode,*BTree; 

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
 
bool InsertAVL(BTree *T,int e,bool *taller)//变量taller反应T长高与否
{
	if(!*T)
	{
		*T=(BTree)malloc(sizeof(BTNode));
		(*T)->data=e;
		(*T)->lchild=(*T)->rchild=NULL;
		(*T)->BF=EH;
		*taller=true;
	}
	else
	{
		if(e==(*T)->data)//不插入
		{
			*taller=false;
			return false; 
		}
		if(e<(*T)->data)
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
 
bool Find(BTree T,int key)
{
	if(!T)
		return false;
	else if(T->data==key)
		return true;
	else if(T->data<key)
		return Find(T->rchild,key);
	else
		return Find(T->lchild,key);
}
 
void Output(BTree T)
{
	if(T)
	{
		printf("%d",T->data);
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
	int i;
	int A[]={3,2,1,4,5,6,7,10,9,8};
	BTree T=NULL;
	bool taller;
	for(i=0;i<sizeof(A)/sizeof(int);i++)
		InsertAVL(&T,A[i],&taller);
	Output(T);
	printf("\n");
	if(Find(T,6))
		printf("6 is find in the AVL tree!\n");
	else 
		printf("6 is not find in the AVL tree!\n"); 
	return 0;
}
 
