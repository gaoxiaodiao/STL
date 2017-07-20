/*
*�ļ�˵��:�����������������
*����:��С��
*����:2017-07-20
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#include<new>

//����new��λ���ʽ����ʼ������
template<typename T1,typename T2>
inline void Construct(T1* p,const T2 &val){
	new (p) T1(val);
}

//������������������Դ��β����(��������)
template<typename T>
inline void Destroy(T *p){
	p->~T();
}

//����һ������
template <class ForwardIterator>
inline void Destroy(ForwardIterator first, ForwardIterator last) {
	_Destroy(first, last, value_type(first));
}

//ͨ����������������ȡ,���ж϶����Ƿ�ΪPOD����,ѡ�����������������
template <class ForwardIterator, class T>
inline void _Destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename _TypeTraits<T>::HasTrivialDestructor TrivialDestructor;
	_DestroyAux(first, last, TrivialDestructor());
}

//��POD����,��������ʱ,��Ҫ������������(��POD������������)
template <class ForwardIterator>
inline void _DestroyAux(ForwardIterator first, ForwardIterator last, _FalseType) {
	for ( ; first<last; ++first)
		Destroy(&*first);
}

//�������ͻ�ǳ������������,����������Ҫ������(POD����)
template <class ForwardIterator> 
inline void _DestroyAux(ForwardIterator, ForwardIterator, __TrueType){}

//�����������ػ��汾
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

