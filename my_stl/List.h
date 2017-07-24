/*
*文件说明:模拟List
*作者:高小调
*日期:2017-7-24
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __LIST_H__
#define __list_H__
#include"Iterator.h"
#include"Alloc.h"
#include"Construct.h"
#include"Uninitialized.h"

template<typename T,typename Ptr,typename Ref>
struct _ListIterator;

template<typename T,typename Alloc=alloc>
class List{
	
public:
	typedef _ListIterator<T,T*,T&> Iterator;
	typedef _ListIterator<const T,const T*,const T&> ConstIterator;
	//list节点定义
	typedef struct ListNode{
		T val;
		ListNode *prev;
		ListNode *next;
		ListNode(const T & v = T() )
			:val(v),
			prev(NULL),
			next(NULL){}
	}Node;
	typedef SimpleAlloc<Node,Alloc> Allocator;
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
		_head->next = _tail;
		_tail->prev = _head;
		Node *cur = l._head->next;
		while(cur!=l._head){
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
		/*
		Node* newNode = BuyNewNode(e);
		_tail->next = newNode;
		newNode->prev = _tail;
		_tail = newNode;*/
		Insert(_tail,e);
	}
	//尾删
	void PopBack(){
		if(!Empty()){
			/*
			Node* tailPrev = _tail->prev;
			DestroyNode(_tail);
			_tail = tailPrev;*/
			Erase(_tail);
		}
	}
	//插入节点
	void Insert(Iterator pos,const T &val){
		Node *newNode = BuyNewNode(val);
		Node *prev = pos.node;
		Node *next = pos.node->next;

		newNode->prev = prev;
		prev->next = newNode;

		newNode->next = next;
		next->prev = newNode;

		if(pos.node == _tail){
			_tail = newNode;
		}

	}
	
	//删除节点
	void Erase(Iterator pos){
		Node* delNodePrev = pos.node->prev;
		Node* delNodeNext = pos.node->next;
		delNodePrev->next = delNodeNext;
		delNodeNext->prev = delNodePrev;
		if(pos == _tail){
			_tail = delNodePrev;
		}
		DestroyNode(pos.node);
	}
	void PushFront(const T &val){
		Insert(End(),val);
	}
	void PopFront(){
		if(!Empty()){
			Erase(Begin());
		}
	}
	//判断链表是否为空
	bool Empty(){
		return _head==_tail;
	}
public:
	Iterator Begin(){
		return _head->next;
	}
	Iterator End(){
		//存在隐式转换,再次调用Iterator的构造函数
		return _head;
	}
protected:
	//创建新节点
	Node* BuyNewNode(const T& val){
		Node* tmp = Allocator::Allocate(sizeof(Node));
		Construct(tmp,val);
		return tmp;
	}
	//释放某个节点
	void DestroyNode(Node* node){
		Destroy(node);
		Allocator::Deallocate(node);
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

template<typename T,typename Ptr,typename Ref>
struct _ListIterator{
	typedef BidirectionalIteratorTag iterator_category;
	typedef T                          value_type;
	typedef ptrdiff_t                   difference_type;
	typedef Ptr                         pointer;
	typedef Ref                         reference;

	typedef _ListIterator<T,Ptr,Ref> Self;

	typename List<T>::Node *node;
	_ListIterator(typename List<T>::Node *n)
		:node(n){}

	Ref operator*(){
		return node->val;
	}

	Ptr operator->(){
		return &operator*();
	}

	bool operator==(Self s){
		return node==s.node;
	}
	bool operator!=(Self s){
		return !operator==(s);
	}

	Self& operator--(){
		node = node->prev;
		return *this;
	}
	Self& operator-(int n){
		while(n--){
			node = node->prev;
		}
		return *this;
	}
	Self operator--(int){
		Self ret(node);
		node = node->prev;
		return ret;
	}

	Self& operator++(){
		node = node->next;
		return *this;
	}
	Self &operator+(int n){
		while(n--){
			node = node->next;
		}
		return *this;
	}
	Self operator++(int){
		Self ret(node);
		node = node->next;
		return ret;
	}
};
#endif