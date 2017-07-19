/*
*�ļ�˵��:����Ŀռ�������
*����:��С��
*����:2017-07-19
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#include<cstdlib>
typedef void(*pFun)();
//һ���ռ��������ļ�ʵ��
class _MallocAllocTemplate{
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
pFun _MallocAllocTemplate::_MallocAllocOOMHandler = 0;
