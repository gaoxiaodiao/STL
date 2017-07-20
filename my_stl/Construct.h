/*
*文件说明:用来构造和析构对象
*作者:高小调
*日期:2017-07-20
*集成开发环境:Microsoft Visual Studio 2010 
*/
#include<new>

//调用new定位表达式来初始化对象
template<typename T1,typename T2>
inline void Construct(T1* p,const T2 &val){
	new (p) T1(val);
}

//调用析构函数进行资源汕尾工作(单个析构)
template<typename T>
inline void Destroy(T *p){
	p->~T();
}

//析构一段区间
template <class ForwardIterator>
inline void Destroy(ForwardIterator first, ForwardIterator last) {
	_Destroy(first, last, value_type(first));
}

//通过迭代器的类型萃取,来判断对象是否为POD类型,选择调用哪种析构函数
template <class ForwardIterator, class T>
inline void _Destroy(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename _TypeTraits<T>::HasTrivialDestructor TrivialDestructor;
	_DestroyAux(first, last, TrivialDestructor());
}

//非POD类型,批量析构时,需要调用析构函数(非POD类型批量析构)
template <class ForwardIterator>
inline void _DestroyAux(ForwardIterator first, ForwardIterator last, _FalseType) {
	for ( ; first<last; ++first)
		Destroy(&*first);
}

//内置类型或浅拷贝对象类型,其析构不需要做事情(POD类型)
template <class ForwardIterator> 
inline void _DestroyAux(ForwardIterator, ForwardIterator, __TrueType){}

//两个析构的特化版本
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

