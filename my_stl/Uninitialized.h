/*
*�ļ�˵��:
*����:��С��
*����:
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#include"Construct.h"
#include<algorithm>

//δ��ʼ���Ŀ�������,�Է�Χ�ڵ�ÿ��������п�������
template <class InputIterator, class ForwardIterator>
inline ForwardIterator UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result) {
  return _UninitializedCopy(first, last, result, ValueType(result));
}

//ͨ��������ȡ��ȡ������,�Բ�ͬ���ͽ��в�ͬ����
template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator _UninitializedCopy(InputIterator first, InputIterator last,ForwardIterator result, T*) {
	typedef typename _TypeTraits<T>::Is_POD_Type is_POD;
	return _UninitializedCopyAux(first, last, result, is_POD());
}

//POD����,ֱ�ӵ����㷨�еĿ���
template <class InputIterator, class ForwardIterator>
inline ForwardIterator _UninitializedCopyAux(InputIterator first, InputIterator last,ForwardIterator result,_TrueType) {
	return copy(first, last, result);
}
//��POD����,��Ҫ����Construct
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

