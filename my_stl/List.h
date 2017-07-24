/*
*文件说明:模拟List
*作者:高小调
*日期:2017-7-24
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __LIST_H__
#define __list_H__
#include"Alloc.h"
#include"Construct.h"
#include"Uninitialized.h"
template<typename T,typename Alloc=alloc>
class List{
	typedef SimpleAlloc<T,Alloc> Allocator;
	//list节点定义
	typedef struct ListNode{
		T val;
		ListNode *prev;
		ListNode *next;
		ListNode(const T &v=T())
			:val(v),
			prev(NULL),
			next(NULL){}
	}Node;
public:
	//构造函数
	List()
		:_head(NULL),
		_tail(NULL){
			_head = _tail = BuyNewNode(T());
			_head->next = _tail;
			_tail->prev = _head;
	}
	//拷贝构造
	List(const List& l)
		:_head(NULL),
		_tail(NULL){
		_head = _tail = BuyNewNode(T());
		Node *cur = l->next;
		while(cur!=l->_head){
			PushBack(cur->val);
			cur = cur->next;
		}
	}
	//赋值运算符重载
	List & operator=(const List& l){
		if(l._head != _head){
			List tmp(l);
			swap(_head,tmp._head);
			swap(_tail,tmp._tail);
		}
		return *this;
	}
	//析构函数
	~List(){
		DestroyList();
		DestroyNode(_head);
		_head = _tail = NULL;
	}
public:
	//尾插
	void PushBack(const T & e){
		Node* newNode = BuyNewNode(e);
		_tail->next = newNode;
		newNode->prev = _tail;
		_tail = newNode;
	}
	//尾删
	void PopBack(){
		if(!Empty()){
			Node* tailPrev = _tail->prev;
			DestroyNode(_tail);
			_tail = tailPrev;
		}
	}
	//判断链表是否为空
	bool Empty(){
		return _head==_tail;
	}
protected:
	//创建新节点
	T* BuyNewNode(const T& val){
		Node* tmp = Allocator::allocate(sizeof(T));
		Construct(tmp,val);
		return tmp;
	}
	//释放某个节点
	void DestroyNode(Node* node){
		Destroy(node);
		Allocator::deallocate(node);
	}
	//释放整个链表(除头节点以外)
	void DestroyList(){
		Node* cur = _head->next;
		Node* prev = NULL;
		while(cur->next!=_head){
			prev = cur;
			DestroyNode(prev);
			cur = cur->next;
		}
		_tail = _head;
		_head->next = _tail;
		_tail->prev = _head;
	}
private:
	Node *_head;
	Node *_tail;
};
#endif