/*
*�ļ�˵��:ģ��List
*����:��С��
*����:2017-7-24
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#ifndef __LIST_H__
#define __list_H__
#include"Alloc.h"
#include"Construct.h"
#include"Uninitialized.h"
template<typename T,typename Alloc=alloc>
class List{
	typedef SimpleAlloc<T,Alloc> Allocator;
	//list�ڵ㶨��
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
	//���캯��
	List()
		:_head(NULL),
		_tail(NULL){
			_head = _tail = BuyNewNode(T());
			_head->next = _tail;
			_tail->prev = _head;
	}
	//��������
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
	//��ֵ���������
	List & operator=(const List& l){
		if(l._head != _head){
			List tmp(l);
			swap(_head,tmp._head);
			swap(_tail,tmp._tail);
		}
		return *this;
	}
	//��������
	~List(){
		DestroyList();
		DestroyNode(_head);
		_head = _tail = NULL;
	}
public:
	//β��
	void PushBack(const T & e){
		Node* newNode = BuyNewNode(e);
		_tail->next = newNode;
		newNode->prev = _tail;
		_tail = newNode;
	}
	//βɾ
	void PopBack(){
		if(!Empty()){
			Node* tailPrev = _tail->prev;
			DestroyNode(_tail);
			_tail = tailPrev;
		}
	}
	//�ж������Ƿ�Ϊ��
	bool Empty(){
		return _head==_tail;
	}
protected:
	//�����½ڵ�
	T* BuyNewNode(const T& val){
		Node* tmp = Allocator::allocate(sizeof(T));
		Construct(tmp,val);
		return tmp;
	}
	//�ͷ�ĳ���ڵ�
	void DestroyNode(Node* node){
		Destroy(node);
		Allocator::deallocate(node);
	}
	//�ͷ���������(��ͷ�ڵ�����)
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