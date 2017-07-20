/*
*文件说明:
*作者:高小调
*日期:
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __UNINITIALIZED_H__
#define __UNINITIALIZED_H__
#include"Construct.h"
#include"Iterator.h"
#include<algorithm>
using std::copy;
//未初始化的拷贝函数,对范围内的每个对象进行拷贝操作
template <class InputIterator, class ForwardIterator>
inline ForwardIterator UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result) {
  return _UninitializedCopy(first, last, result, ValueType(result));//用到了迭代器萃取
}

//通过类型萃取获取其类型,对不同类型进行不同处理
template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator _UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result, T*a) {
	typedef typename _TypeTraits<T>::Is_POD_Type is_POD;			//类型萃取
	return _UninitializedCopyAux(first, last, result, is_POD());
}

//POD类型,直接调用算法中的拷贝,浅拷贝
template <class InputIterator, class ForwardIterator>
inline ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_TrueType) {
	return copy(first, last, result);
}
//非POD类型,需要调用Construct,相当于一个一个的拷贝构造对象
template <class InputIterator, class ForwardIterator>
ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_FalseType) {
	ForwardIterator cur = result;
	for ( ; first != last; ++first, ++cur)
		Construct(&*cur, *first);
	return cur;
}

//两个"特化"拷贝
inline char* UninitializedCopy(const char* first, const char* last,char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}
inline wchar_t* UninitializedCopy(const wchar_t* first, const wchar_t* last,wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

/*
//随机访问迭代器
template <class RandomAccessIterator, class Size, class ForwardIterator>
inline pair<RandomAccessIterator, ForwardIterator> _UninitializedCopyN(RandomAccessIterator first, Size count,ForwardIterator result,RandomAccessIteratorTag) {
	RandomAccessIterator last = first + count;
	return make_pair(last, UninitializedCopy(first, last, result));
}

//只读迭代器
template <class InputIterator, class Size, class ForwardIterator>
pair<InputIterator, ForwardIterator> _UninitializedCopyN(InputIterator first, Size count, ForwardIterator result,InputIteratorTag) {
	ForwardIterator cur = result;
	for ( ; count > 0 ; --count, ++first, ++cur) 
		Construct(&*cur, *first);
	return pair<InputIterator, ForwardIterator>(first, cur);
}

//其他略...
*/
#endif