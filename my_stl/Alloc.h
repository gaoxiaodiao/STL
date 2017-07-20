/*
*文件说明:简洁版的空间配置器
*作者:高小调
*日期:2017-07-19
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __ALLOC_H__
#define __ALLOC_H__
#include<cstdlib>
///////////////一级空间配置器的简单实现///////////////////
class _MallocAllocTemplate{
	typedef void(*pFun)();
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
_MallocAllocTemplate::pFun _MallocAllocTemplate::_MallocAllocOOMHandler = 0;
///////////////////一级空间配置器结束////////////////////
typedef _MallocAllocTemplate MallocAlloc;
///////////////////二级空间配置器开始///////////////////
class _DefaultAllocTemplate{
	enum {_ALIGN = 8};						//小内存大小
	enum {_MAX_BYTES = 128};				//超出_MAX_BYTES视为大内存
	enum {_FFREELISTS = _MAX_BYTES/_ALIGN};	//自由链表种类个数
	//定义这个东西只是为了将小区间内存插入自由链表
	union obj{
		union obj * freeListLink;
		char clientData[1];	//并不知道这个东西是干啥的
	};
	//与8的倍数对齐
	//例如需要10个字节
	//(10+8-1) & ~7 = 17 & ~7
	//1 0001
	//1 1000
	//最终与8对齐后为16,表示需要去找16字节的小内存
	static size_t ROUND_UP(size_t bytes){
		return ((bytes+_ALIGN-1) & ~(_ALIGN - 1));
	}
	//根据所需内存,寻找其对应自由链表的下标
	static size_t FREELIST_INDEX(size_t bytes){
		return (bytes+_ALIGN-1)/_ALIGN - 1;
	}
	//将内存池中的内存填充入自由链表中
	static void*Refill(size_t n);
	//申请大块内存
	static char*ChunkAlloc(size_t size,int &nobjs);
public:
	static void* Allocate(size_t n){
		if(n > _MAX_BYTES){
			//如果所申请内存大于MAX_BYTES(128),则调用
			//一级空间配置器
			return MallocAlloc::Allocate(n);
		}
		int index = FREELIST_INDEX(n);
		obj * ret = 0;
		ret = freeList[index];
		if(ret == 0){
			//当前自由链表中没有节点,需要从大块内存中拿入
			void *r = Refill(ROUND_UP(n));
			return r;
		}
		//当前自由链表中有节点,采取头删方式,返回该节点
		freeList[index] = freeList[index]->freeListLink;
		return ret;
	}
	static void Deallocate(void*p,size_t n){
		if(n > _MAX_BYTES){
			//通过一级空间配置器来释放
			MallocAlloc::Deallocate(p,n);
			return ;
		}
		//将所释放的节点以头插方式挂回自由链表
		obj* newHead = (obj*)p;
		int index = FREELIST_INDEX(n);
		newHead->freeListLink = freeList[index];
		freeList[index] = newHead;
	}
private:
	static obj* freeList[_FFREELISTS];	//自由链表
	static char* startFree;				//内存池开始
	static char* endFree;				//内存池结束
	static size_t heapSize;				//向系统申请总内存大小
};
char* _DefaultAllocTemplate::startFree = 0;
char* _DefaultAllocTemplate::endFree = 0;
_DefaultAllocTemplate::obj* _DefaultAllocTemplate::freeList[_FFREELISTS]={0};
size_t _DefaultAllocTemplate::heapSize = 0;

typedef _DefaultAllocTemplate alloc;

//从内存池中获取内存
char * _DefaultAllocTemplate::ChunkAlloc(size_t size,int &nobjs){
	char *ret = 0;
	size_t totalBytes = size*nobjs;			//总共所需字节
	size_t leftBytes = endFree - startFree;	//内存池中剩余字节
	if(leftBytes >= totalBytes){
		//内存池中剩余内存足够给所需内存
		ret = startFree;
		startFree += totalBytes;
		return ret;
	}else if(leftBytes >= size){
		//至少可以分配一个出来
		nobjs = leftBytes/size;		//计算可以分配几个
		totalBytes = size*nobjs;	//计算可以分配的总大小
		ret = startFree;
		startFree += totalBytes;
		return ret;
	}else{
		//内存池一个对象都分配不出来,扩充内存池
		size_t bytesToGet = 2 * totalBytes + ROUND_UP(heapSize>>4);
		if(leftBytes > 0){
			//内存池还有剩余,将其挂入自由链表
			int index = FREELIST_INDEX(leftBytes);
			((obj*)startFree)->freeListLink = freeList[index];
			freeList[index] = (obj*)startFree;
		}
		startFree = (char*)malloc(bytesToGet);
		if(0 == startFree){
			//系统没有这么多内存,找更大块自由链表去要
			int i = 0;
			int index = 0;
			obj* p;
			//从当前内存开始,遍历后续自由链表
			for(i=size; i<=_MAX_BYTES; i+=_ALIGN){
				index = FREELIST_INDEX(i);
				p = freeList[index];
				if(0 != p){
					//将某个自由链表中的内存拿下来,放到内存池里
					freeList[index] = p->freeListLink;
					startFree = (char*)p;
					endFree = startFree + i;
					//继续递归分配
					return (ChunkAlloc(size,nobjs));
				}
			}
			endFree = 0;
			//山穷水尽,更大自由链表中也没有东西了
			//再抱有一丝丝幻想,也许还能分配出内存...
			//更高的概率在此处会抛出一个异常
			startFree = (char*)MallocAlloc::Allocate(bytesToGet);
		}
		heapSize += bytesToGet;
		endFree = startFree + bytesToGet;
		return ChunkAlloc(size,nobjs);
	}
}
//返回所需的小块内存(将剩下的内存挂入自由链表)
void * _DefaultAllocTemplate::Refill(size_t n){
	int nobjs = 20;		//多申请19个小块内存
	char * chunk = ChunkAlloc(n,nobjs);		//从内存池中获取内存
	int index = FREELIST_INDEX(n);
	obj *curObj,*nextObj;
	int i = 0;
	
	if(nobjs == 1){
		//从大块内存中只取到一个	
		return chunk;
	}
	//将一个第一块小块内存返回
	void *ret = chunk;
	//剩下的小块内存挂入自由链表
	freeList[index] = nextObj = (obj*)(chunk + n);
	for(i=1; i!=nobjs-1; ++i){
		curObj = nextObj;
		nextObj = (obj*)((char*)nextObj + n);
		curObj->freeListLink = nextObj;
	}
	curObj->freeListLink = 0;
	return ret;
}
//一套符合STL规格接口的类
template<typename T,typename Alloc>
class SimpleAlloc{
public:
	static T* Allocate(size_t n){
		return n==0?0:(T*)Alloc::Allocate(n);
	}
	static T* Allocate(void){
		return (T*)Alloc::Allocate(sizeof(T));
	}
	static void Deallocate(void *p){
		Alloc::Deallocate(p,sizeof(T));
	}
	static void Deallocate(void *p,size_t n){
		if(n!=0)Alloc::Deallocate(p,n*sizeof(T));
	}
};
#endif