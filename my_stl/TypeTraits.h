/*
*�ļ�˵��:������ȡ
*����:��С��
*����:2017-07-20
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#ifndef __TYPETRAITS_H__
#define __TYPETRAITS_H__
struct _TrueType {};
struct _FalseType {};

template<typename T>
struct _TypeTraits{
	//��ʱ����Ϊһ����Ա
	typedef _FalseType Is_POD_Type;
	typedef _FalseType HasTrivialDestructor;
};

template<>
struct _TypeTraits<int>{
	typedef _TrueType Is_POD_Type;
	typedef _TrueType HasTrivialDestructor;
};

template<>
struct _TypeTraits<unsigned int>{
	typedef _TrueType    Is_POD_Type;
	typedef _TrueType HasTrivialDestructor;
};

template<>
struct _TypeTraits<char>{
	typedef _TrueType    Is_POD_Type;
};

template<>
struct _TypeTraits<unsigned char>{
	typedef _TrueType    Is_POD_Type;
	typedef _TrueType HasTrivialDestructor;
};
//......

#endif