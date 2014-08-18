/***********************************************************************
  实验九  第 11 题
  题目：建立按C++成绩排序的有序链表
  要求： 　
  
  注意： 
***********************************************************************/
#include <iostream.h>
#include <string.h>

struct student
{
	int  num;      //学号
	char name[20];  //姓名  //不能定义成 char  name;
	int  age;       //年龄
	int  cscore;    //C++成绩
	student *next;  //下一结点指针
};

student *Insert(student *head, student *p)
{
	student *p1,*p2;
	if(head==NULL) //插入第一个结点
	{
		head=p;
		p->next=NULL;
		return(head);
	}
	if(head->cscore >= p->cscore)//插在首结点之前
	{
		p->next=head;
		head=p;
		return(head);
	}
	p2=p1=head;
	while(p2->next && (p2->cscore < p->cscore) )
	{
		p1=p2;
		p2=p2->next;
	}
	if(p2->cscore < p->cscore) //插在末尾
	{
		p2->next=p;
		p->next=NULL;
	}
	else{  //插在p1指向的结点之后，p2指向的结点之前
		p->next=p2;
		p1->next=p;
	}
	return(head);
}

student *Create(void)
{
	student *p,*head=0;
	int num;
	cout<<"产生一条排序链表，请输入学号、姓名、年龄、C++成绩，以-1结束:\n";
	cin>>num;
	while(num != -1)
	{
		p=new student;
		p->num = num;
		cin >> p->name; //直接输入即可。若前面有 char name[10];
		cin >> p->age;                 //  cin>>name;
		cin >> p->cscore;             //  strcpy(p->name,name);
		head=Insert(head,p);
		cin>>num;
	}
	return(head);
}

void Print(const student *head)
{
	const student *p;
	p=head;
	cout<<"\n链表上各结点的数据为:\n";
	while(p!=NULL)
	{
		cout<<p->num<<'\t'
		    <<p->name<<'\t'
		    <<p->age<<'\t'
		    <<p->cscore<<'\n';
		p=p->next;
	}
	cout<<'\n';
}

void deletechain(student *h)
{
	student *p;
	while(h)
	{
		p=h;	
		h=h->next;
		delete p;
	}
}

void main( )
{
	student *head;
	head=Create( );
	Print(head);
	deletechain(head);
}

