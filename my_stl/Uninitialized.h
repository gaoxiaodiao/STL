/*
*�ļ�˵��:
*����:��С��
*����:
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#ifndef __UNINITIALIZED_H__
#define __UNINITIALIZED_H__
#include"Construct.h"
#include"Iterator.h"
#include<algorithm>
using std::copy;
//δ��ʼ���Ŀ�������,�Է�Χ�ڵ�ÿ��������п�������
template <class InputIterator, class ForwardIterator>
inline ForwardIterator UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result) {
  return _UninitializedCopy(first, last, result, ValueType(result));//�õ��˵�������ȡ
}

//ͨ��������ȡ��ȡ������,�Բ�ͬ���ͽ��в�ͬ����
template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator _UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result, T*a) {
	typedef typename _TypeTraits<T>::Is_POD_Type is_POD;			//������ȡ
	return _UninitializedCopyAux(first, last, result, is_POD());
}

//POD����,ֱ�ӵ����㷨�еĿ���,ǳ����
template <class InputIterator, class ForwardIterator>
inline ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_TrueType) {
	return copy(first, last, result);
}
//��POD����,��Ҫ����Construct,�൱��һ��һ���Ŀ����������
template <class InputIterator, class ForwardIterator>
ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_FalseType) {
	ForwardIterator cur = result;
	for ( ; first != last; ++first, ++cur)
		Construct(&*cur, *first);
	return cur;
}

//����"�ػ�"����
inline char* UninitializedCopy(const char* first, const char* last,char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}
inline wchar_t* UninitializedCopy(const wchar_t* first, const wchar_t* last,wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

/*
//������ʵ�����
template <class RandomAccessIterator, class Size, class ForwardIterator>
inline pair<RandomAccessIterator, ForwardIterator> _UninitializedCopyN(RandomAccessIterator first, Size count,ForwardIterator result,RandomAccessIteratorTag) {
	RandomAccessIterator last = first + count;
	return make_pair(last, UninitializedCopy(first, last, result));
}

//ֻ��������
template <class InputIterator, class Size, class ForwardIterator>
pair<InputIterator, ForwardIterator> _UninitializedCopyN(InputIterator first, Size count, ForwardIterator result,InputIteratorTag) {
	ForwardIterator cur = result;
	for ( ; count > 0 ; --count, ++first, ++cur) 
		Construct(&*cur, *first);
	return pair<InputIterator, ForwardIterator>(first, cur);
}

//������...
*/
#endif