/*
*文件说明:简洁版的空间配置器
*作者:高小调
*日期:2017-07-19
*集成开发环境:Microsoft Visual Studio 2010 
*/
#include<cstdlib>
typedef void(*pFun)();
//一级空间配置器的简单实现
class _MallocAllocTemplate{
public:
	//对malloc进行简单的封装,当分配内存失败时,采用set_new_handler机制
	static void *Allocate(size_t n){
		void *ret = malloc(n);
		if(ret==NULL){
			ret = OOM_Malloc(n);
		}
		return ret;
	}
	//对free的封装
	static void Deallocate(void* p,size_t n){
		free(p);
	}
	//同上
	static void *Reallocate(void*p,size_t ,size_t newsize){
		void *ret = realloc(p,newsize);
		if(ret==NULL){
			ret = OOM_Realloc(p,newsize);
		}
		return ret;
	}
	//设置句柄函数
	pFun SetMallocHandler(pFun newHandler){
		pFun oldHandler = _MallocAllocOOMHandler;
		_MallocAllocOOMHandler = newHandler;
		return oldHandler;
	}
private:
	//具有set_new_handler机制的malloc
	static void *OOM_Malloc(size_t n){
		void *ret = 0;
		while(1){
			if(_MallocAllocOOMHandler == 0){
				//此处抛出bad_alloc异常
			}
			_MallocAllocOOMHandler();	//尝试释放一些内存
			ret = malloc(n);
			if(ret!=0){return ret;}
		}
	}
	//具有set_new_handler机制的realloc
	static void* OOM_Realloc(void *p,size_t n){
		void*ret = 0;
		while(1){
			if(_MallocAllocOOMHandler==0){
				//此处抛出bad_alloc异常
			}
			_MallocAllocOOMHandler();
			ret = realloc(p,n);
			if(ret!=0){
				return ret;
			}
		}
	}
	//句柄函数,内存不足时,调用该函数可以分配内存,默认为空
	static pFun _MallocAllocOOMHandler;
};
pFun _MallocAllocTemplate::_MallocAllocOOMHandler = 0;
