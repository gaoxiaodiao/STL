/*
*文件说明:测试空间配置器
*作者:高小调
*日期:2017-07-19
*集成开发环境:Microsoft Visual Studio 2010 
*/

#include<iostream>
#include"Uninitialized.h"
#include"Alloc.h"
#include<string>
#include"Vector.h"
#include<vector>
#include"List.h"
using std::vector;
using std::string;
using std::cout;
using std::endl;
void TestAlloc(){
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
void TestUninitializedCopy(){
	//POD类型
	int arr[10] = {1,3,5,7,9,2,4,6,8,0};
	int dst[10];
	int *start = arr;
	int *end = arr+10;
	UninitializedCopy(start,end,dst);
	for(int i=0; i<10; ++i){
		cout<<dst[i]<<" ";
	}
	cout<<endl;
	//非POD类型
	string arr2[3];
	arr2[0] = "123456789";
	arr2[1] = "135792468";
	arr2[2] = "987654321";
	string dst2[3];
	UninitializedCopy(arr2,arr2+3,dst2);
	arr2[0] = "1111111111";
	arr2[1] = "2222222222";
	arr2[2] = "3333333333";
	for(int i=0; i<3; ++i){
		cout<<dst2[i].c_str()<<endl;
	}
}
void TestVector(){
	
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushFront(0);
	Vector<int>::Iterator it = v.Begin();
	while(it != v.End()){
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	
	Vector<string> v2;
	v2.PushBack("123456789");
	v2.PushBack("987654321");
	v2.PushFront("1111111111");
	Vector<string>::Iterator it2 = v2.Begin();
	while(it2 != v2.End()){
		cout<<*it2<<" ";
		++it2;
	}
	cout<<endl;
	v2.Erase(v2.Begin()+1);
	it2 = v2.Begin();
	while(it2 != v2.End()){
		cout<<*it2<<" ";
		++it2;
	}
}
void TestList(){
	List<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushFront(0);
	List<int>::Iterator it = v.Begin();
	while(it != v.End()){
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	v.PopBack();
	it = v.Begin();
	while(it != v.End()){
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;
	v.PopFront();
	it = v.Begin();
	while(it != v.End()){
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;


	List<string> v2;
	v2.PushBack("123456789");
	v2.PushBack("987654321");
	v2.PushFront("1111111111");
	List<string>::Iterator it2 = v2.Begin();
	while(it2 != v2.End()){
		cout<<*it2<<" ";
		++it2;
	}
	cout<<endl;
	v2.Erase(v2.Begin()+1);
	it2 = v2.Begin();
	while(it2 != v2.End()){
		cout<<*it2<<" ";
		++it2;
	}
}
int main(){
	TestList();
	//TestVector();
	//TestUninitializedCopy();
	//TestAlloc();
	return 0;
}