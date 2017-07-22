/*
*文件说明:类型萃取
*作者:高小调
*日期:2017-07-20
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __TYPETRAITS_H__
#define __TYPETRAITS_H__
struct _TrueType {};
struct _FalseType {};

template<typename T>
struct _TypeTraits{
	//暂时精简为一个成员
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