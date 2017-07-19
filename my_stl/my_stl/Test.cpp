/*
*文件说明:测试空间配置器
*作者:高小调
*日期:2017-07-19
*集成开发环境:Microsoft Visual Studio 2010 
*/
#include<iostream>
#include"Alloc.h"
using std::cout;
using std::endl;
void Test(){
	int *a = SimpleAlloc<int,alloc>::Allocate();
	int *arr = SimpleAlloc<int,alloc>::Allocate(10);
	*a = 100;
	cout<<*a<<endl;
	for(int i=0; i<10; ++i){
		arr[i] = i;
	}
	for(int i=0; i<10; ++i){
		cout<<arr[i]<<" "; 
	}
	cout<<endl;

}
int main(){
	Test();
	return 0;
}