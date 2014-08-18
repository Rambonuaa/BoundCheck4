/***********************************************************************
  实验九  第 4 题
  题目：将10 个整 数用插入法排序 后输出
  
  方法：
        1. 从前往后扫描 
		2. 从后往前扫描
***********************************************************************/
#include <iostream.h>

//解1，从前往后扫描
void InsertSort(float *p, float f, int n)// p: 数组起始地址，
                                         // f: 待插入数值，
                                         // n: 插入前数组中元素个数
{   
	float *begin, *p1;

    if(n==0){ *p=f; return; }// 插入数组第0个元素
                             // 此句可不要，因下述语句包含了此情况
                             // 但从程序的可读性角度来看，亦可保留

    for(begin=p; p<(begin+n); p++)//找到f应插入的位置
            if(f<*p) break;  

    p1=p; //p1是要插入元素的位置指针

    for(p=(begin+n); p>p1; p--)//向后挪动p1指针以后的元素值
            *p = *(p-1);       //注意挪动方向 

    *p1=f;
}

void main()
{   
	float a[10], f;
    cout << "任意输入十个数值:\n";
    for(int i=0; i<10; i++)
	{
        cin >> f;
        InsertSort(a, f, i); //向已有i个元素的a数组中插入f
	}

    cout << "插入排序后输出:\n";
    for(i=0; i<10; i++)
          cout << *(a+i) << '\t';
    cout << "\n";
}


//解2，从后往前扫描
/*
void InsertSort(float *p, float f, int n)// p: 数组起始地址，
                                         // f: 待插入数值，
                                         // n: 插入前数组中元素个数
{   float *begin=p;

    if(n==0){ *p=f; return; }// 插入数组第0个元素
                             // 此句可不要，因下述语句包含了此情况
                             // 但从程序的可读性角度来看，亦可保留

    p=p+n-1; //让p指向数组最后一个元素

    for( ; p>=begin; p--)
       if(f<*p) *(p+1)=*p;
       else break;
    *(p+1)=f;
}
*/


/*

// 在被调函数中，用数组访问的方式解决
void InsertSort(float *p, float f, int n)// p: 数组起始地址，
{
	for(int i=0; i<n; i++)
		if(p[i]>f) break;
	for(int s=n; s>i; s--)
		p[s]=p[s-1];
	p[i]=f;
}

*/

