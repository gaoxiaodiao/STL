/*
*�ļ�˵��:����Ŀռ�������
*����:��С��
*����:2017-07-19
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#ifndef __ALLOC_H__
#define __ALLOC_H__
#include<cstdlib>
///////////////һ���ռ��������ļ�ʵ��///////////////////
class _MallocAllocTemplate{
	typedef void(*pFun)();
public:
	//��malloc���м򵥵ķ�װ,�������ڴ�ʧ��ʱ,����set_new_handler����
	static void *Allocate(size_t n){
		void *ret = malloc(n);
		if(ret==NULL){
			ret = OOM_Malloc(n);
		}
		return ret;
	}
	//��free�ķ�װ
	static void Deallocate(void* p,size_t n){
		free(p);
	}
	//ͬ��
	static void *Reallocate(void*p,size_t ,size_t newsize){
		void *ret = realloc(p,newsize);
		if(ret==NULL){
			ret = OOM_Realloc(p,newsize);
		}
		return ret;
	}
	//���þ������
	pFun SetMallocHandler(pFun newHandler){
		pFun oldHandler = _MallocAllocOOMHandler;
		_MallocAllocOOMHandler = newHandler;
		return oldHandler;
	}
private:
	//����set_new_handler���Ƶ�malloc
	static void *OOM_Malloc(size_t n){
		void *ret = 0;
		while(1){
			if(_MallocAllocOOMHandler == 0){
				//�˴��׳�bad_alloc�쳣
			}
			_MallocAllocOOMHandler();	//�����ͷ�һЩ�ڴ�
			ret = malloc(n);
			if(ret!=0){return ret;}
		}
	}
	//����set_new_handler���Ƶ�realloc
	static void* OOM_Realloc(void *p,size_t n){
		void*ret = 0;
		while(1){
			if(_MallocAllocOOMHandler==0){
				//�˴��׳�bad_alloc�쳣
			}
			_MallocAllocOOMHandler();
			ret = realloc(p,n);
			if(ret!=0){
				return ret;
			}
		}
	}
	//�������,�ڴ治��ʱ,���øú������Է����ڴ�,Ĭ��Ϊ��
	static pFun _MallocAllocOOMHandler;
};
_MallocAllocTemplate::pFun _MallocAllocTemplate::_MallocAllocOOMHandler = 0;
///////////////////һ���ռ�����������////////////////////
typedef _MallocAllocTemplate MallocAlloc;
///////////////////�����ռ���������ʼ///////////////////
class _DefaultAllocTemplate{
	enum {_ALIGN = 8};						//С�ڴ��С
	enum {_MAX_BYTES = 128};				//����_MAX_BYTES��Ϊ���ڴ�
	enum {_FFREELISTS = _MAX_BYTES/_ALIGN};	//���������������
	//�����������ֻ��Ϊ�˽�С�����ڴ������������
	union obj{
		union obj * freeListLink;
		char clientData[1];	//����֪����������Ǹ�ɶ��
	};
	//��8�ı�������
	//������Ҫ10���ֽ�
	//(10+8-1) & ~7 = 17 & ~7
	//1 0001
	//1 1000
	//������8�����Ϊ16,��ʾ��Ҫȥ��16�ֽڵ�С�ڴ�
	static size_t ROUND_UP(size_t bytes){
		return ((bytes+_ALIGN-1) & ~(_ALIGN - 1));
	}
	//���������ڴ�,Ѱ�����Ӧ����������±�
	static size_t FREELIST_INDEX(size_t bytes){
		return (bytes+_ALIGN-1)/_ALIGN - 1;
	}
	//���ڴ���е��ڴ����������������
	static void*Refill(size_t n);
	//�������ڴ�
	static char*ChunkAlloc(size_t size,int &nobjs);
public:
	static void* Allocate(size_t n){
		if(n > _MAX_BYTES){
			//����������ڴ����MAX_BYTES(128),�����
			//һ���ռ�������
			return MallocAlloc::Allocate(n);
		}
		int index = FREELIST_INDEX(n);
		obj * ret = 0;
		ret = freeList[index];
		if(ret == 0){
			//��ǰ����������û�нڵ�,��Ҫ�Ӵ���ڴ�������
			void *r = Refill(ROUND_UP(n));
			return r;
		}
		//��ǰ�����������нڵ�,��ȡͷɾ��ʽ,���ظýڵ�
		freeList[index] = freeList[index]->freeListLink;
		return ret;
	}
	static void Deallocate(void*p,size_t n){
		if(n > _MAX_BYTES){
			//ͨ��һ���ռ����������ͷ�
			MallocAlloc::Deallocate(p,n);
			return ;
		}
		//�����ͷŵĽڵ���ͷ�巽ʽ�һ���������
		obj* newHead = (obj*)p;
		int index = FREELIST_INDEX(n);
		newHead->freeListLink = freeList[index];
		freeList[index] = newHead;
	}
private:
	static obj* freeList[_FFREELISTS];	//��������
	static char* startFree;				//�ڴ�ؿ�ʼ
	static char* endFree;				//�ڴ�ؽ���
	static size_t heapSize;				//��ϵͳ�������ڴ��С
};
char* _DefaultAllocTemplate::startFree = 0;
char* _DefaultAllocTemplate::endFree = 0;
_DefaultAllocTemplate::obj* _DefaultAllocTemplate::freeList[_FFREELISTS]={0};
size_t _DefaultAllocTemplate::heapSize = 0;

typedef _DefaultAllocTemplate alloc;

//���ڴ���л�ȡ�ڴ�
char * _DefaultAllocTemplate::ChunkAlloc(size_t size,int &nobjs){
	char *ret = 0;
	size_t totalBytes = size*nobjs;			//�ܹ������ֽ�
	size_t leftBytes = endFree - startFree;	//�ڴ����ʣ���ֽ�
	if(leftBytes >= totalBytes){
		//�ڴ����ʣ���ڴ��㹻�������ڴ�
		ret = startFree;
		startFree += totalBytes;
		return ret;
	}else if(leftBytes >= size){
		//���ٿ��Է���һ������
		nobjs = leftBytes/size;		//������Է��伸��
		totalBytes = size*nobjs;	//������Է�����ܴ�С
		ret = startFree;
		startFree += totalBytes;
		return ret;
	}else{
		//�ڴ��һ�����󶼷��䲻����,�����ڴ��
		size_t bytesToGet = 2 * totalBytes + ROUND_UP(heapSize>>4);
		if(leftBytes > 0){
			//�ڴ�ػ���ʣ��,���������������
			int index = FREELIST_INDEX(leftBytes);
			((obj*)startFree)->freeListLink = freeList[index];
			freeList[index] = (obj*)startFree;
		}
		startFree = (char*)malloc(bytesToGet);
		if(0 == startFree){
			//ϵͳû����ô���ڴ�,�Ҹ������������ȥҪ
			int i = 0;
			int index = 0;
			obj* p;
			//�ӵ�ǰ�ڴ濪ʼ,����������������
			for(i=size; i<=_MAX_BYTES; i+=_ALIGN){
				index = FREELIST_INDEX(i);
				p = freeList[index];
				if(0 != p){
					//��ĳ�����������е��ڴ�������,�ŵ��ڴ����
					freeList[index] = p->freeListLink;
					startFree = (char*)p;
					endFree = startFree + i;
					//�����ݹ����
					return (ChunkAlloc(size,nobjs));
				}
			}
			endFree = 0;
			//ɽ��ˮ��,��������������Ҳû�ж�����
			//�ٱ���һ˿˿����,Ҳ���ܷ�����ڴ�...
			//���ߵĸ����ڴ˴����׳�һ���쳣
			startFree = (char*)MallocAlloc::Allocate(bytesToGet);
		}
		heapSize += bytesToGet;
		endFree = startFree + bytesToGet;
		return ChunkAlloc(size,nobjs);
	}
}
//���������С���ڴ�(��ʣ�µ��ڴ������������)
void * _DefaultAllocTemplate::Refill(size_t n){
	int nobjs = 20;		//������19��С���ڴ�
	char * chunk = ChunkAlloc(n,nobjs);		//���ڴ���л�ȡ�ڴ�
	int index = FREELIST_INDEX(n);
	obj *curObj,*nextObj;
	int i = 0;
	
	if(nobjs == 1){
		//�Ӵ���ڴ���ֻȡ��һ��	
		return chunk;
	}
	//��һ����һ��С���ڴ淵��
	void *ret = chunk;
	//ʣ�µ�С���ڴ������������
	freeList[index] = nextObj = (obj*)(chunk + n);
	for(i=1; i!=nobjs-1; ++i){
		curObj = nextObj;
		nextObj = (obj*)((char*)nextObj + n);
		curObj->freeListLink = nextObj;
	}
	curObj->freeListLink = 0;
	return ret;
}
//һ�׷���STL���ӿڵ���
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