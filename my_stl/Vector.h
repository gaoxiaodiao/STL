/*
*文件说明:模拟实现Vector
*作者:高小调
*日期:2017-07-21
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __VECTOR_H__
#define __VECTOR_H__
#include"Alloc.h"
#include"Construct.h"
#include"Uninitialized.h"
template<typename T,typename Alloc=alloc>
class Vector{
	typedef SimpleAlloc<T,Alloc> Allocator;
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
public:
	Vector():_start(0),_finish(0),_endOfStorage(0){}
	Vector(const Vector& v):_start(0),_finish(0),endOfStorage(0){
		size_t n = v.End()-v.Begin();
		_start = AllocateAndCopy(n,v.Begin(),v.End());
		_finish = _start + n;
		_endOfStorage = _finish;
	}
	Vector & operator=(const Vector& v){
		if(v._start == _start){
			Vector<T> tmp(v);
			swap(*this,tmp);
		}
		return *this;
	}
	~Vector(){Deallocate();}

public:
	void PushBack(const T &val){
		_CheckCapacity();//这句必须加上,防止第一次进来迭代器为0
		Insert(End(),val);
	}
	void PushFront(const T & val){
		_CheckCapacity();
		Insert(Begin(),val);
	}
	void Insert(Iterator pos,const T & val){
		_CheckCapacity();
		Iterator end = End();
		while(pos < end){
			//*end = *(end-1); 错误!
			Construct(&*end,*(end-1));
			end--;
		}
		Construct(&*pos,val);//*pos = val; 错误!
		_finish++;
	}
	void PopFront(){
		if(Empty()){
			return ;
		}
		Erase(Begin()):
	}
	void PopBack(){
		if(Empty()){
			return ;
		}
		Erase(End()-1);
	}
	Iterator Erase(Iterator pos){
		Iterator end = End();
		while(pos < end-1){
			Construct(&*pos,*(pos+1));
			pos++;
		}
		--_finish;
		Destroy(_finish);
		return pos;
	}
	int Size(){
		return _finish - _start;
	}
	bool Empty(){
		return Size() == 0;
	}
protected:
	//检测并进行扩容
	void _CheckCapacity(){
		if(_start == 0){
			//Vector为空
			int capacity = 10;
			_start = Allocator::Allocate(capacity*sizeof(T));
			_finish = _start;
			_endOfStorage = _start + capacity;
		}else if( _finish >= _endOfStorage){
			//需要扩容,先计算扩容后的容量
			int oldCapacity = _endOfStorage - _start;
			int newCapacity = oldCapacity*2+3;
			int size = End() - Begin();
			//将原来的数据拷贝到新容量中
			Iterator newBase = AllocateAndCopy(newCapacity,Begin(),End());
			//析构现有数据
			Deallocate();
			//改变指针指向
			_start = newBase;
			_finish = newBase + size;
			_endOfStorage = newBase + newCapacity;
		}
	
	}
	Iterator AllocateAndCopy(size_t n,Iterator first,Iterator end){
		Iterator ret = Allocator::Allocate(n);
		UninitializedCopy(first,end,ret);
		return ret;
	}
	void Deallocate(){
		Destroy(_start,_finish);						//调用析构函数
		Allocator::Deallocate(_start,_endOfStorage-_start);	//释放空间
		_start = _finish = _endOfStorage = 0;				//
	}
public:
	Iterator Begin(){
		return _start;
	}
	Iterator End(){
		return _finish;
	}
private:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};
#endif