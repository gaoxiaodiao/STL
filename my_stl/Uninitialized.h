/*
*文件说明:
*作者:高小调
*日期:
*集成开发环境:Microsoft Visual Studio 2010 
*/
#include"Construct.h"
#include<algorithm>

//未初始化的拷贝函数,对范围内的每个对象进行拷贝操作
template <class InputIterator, class ForwardIterator>
inline ForwardIterator UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result) {
  return _UninitializedCopy(first, last, result, ValueType(result));
}

//通过类型萃取获取其类型,对不同类型进行不同处理
template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator _UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result, T*) {
	typedef typename _TypeTraits<T>::Is_POD_Type is_POD;
	return _UninitializedCopyAux(first, last, result, is_POD());
}

//POD类型,直接调用算法中的拷贝
template <class InputIterator, class ForwardIterator>
inline ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_TrueType) {
	return copy(first, last, result);
}
//非POD类型,需要调用Construct
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

