/*
*文件说明:迭代器部分
*作者:高小调
*日期:2017-07-20
*集成开发环境:Microsoft Visual Studio 2010 
*/
#ifndef __ITERATOR_H__
#define __ITERATOR_H__
//五种迭代器标签(用作类型萃取)
struct InputIteratorTag{};												//只读迭代器
struct OutputIteratorTag{};												//只写迭代器
struct ForwardIteratorTag:public InputIteratorTag {};					//前向迭代器
struct BidirectionalIteratorTag : public ForwardIteratorTag {};			//反向迭代器
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};	//随机访问迭代器

//////////////////////////五种迭代器的定义开始//////////////////////////
template <class T, class Distance>
struct InputIterator {
	typedef InputIteratorTag iterator_category;
	typedef T                  value_type;
	typedef Distance           difference_type;
	typedef T*                 pointer;
	typedef T&                 reference;
};

struct OutputIterator {
	typedef OutputIteratorTag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;
};

template <class T, class Distance>
struct ForwardIterator {
	typedef ForwardIteratorTag iterator_category;
	typedef T                    value_type;
	typedef Distance             difference_type;
	typedef T*                   pointer;
	typedef T&                   reference;
};

template <class T, class Distance>
struct BidirectionalIterator {
	typedef BidirectionalIteratorTag iterator_category;
	typedef T                          value_type;
	typedef Distance                   difference_type;
	typedef T*                         pointer;
	typedef T&                         reference;
};

template <class T, class Distance>
struct RandomAccessIterator {
	typedef RandomAccessIteratorTag iterator_category;
	typedef T                          value_type;
	typedef Distance                   difference_type;
	typedef T*                         pointer;
	typedef T&                         reference;
};
//////////////////////////五种迭代器的定义结束//////////////////////////



////////////////////迭代器的萃取///////////////////////////
template <class Iterator>
struct IteratorTraits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
};

//萃取获得iterator_category(迭代器种类)
template <class Iterator>
inline typename IteratorTraits<Iterator>::iterator_category IteratorCategory(const Iterator&) {
	typedef typename IteratorTraits<Iterator>::iterator_category category;
	return category();
}
//萃取获得difference_type(两个迭代器之间的距离)
template <class Iterator>
inline typename IteratorTraits<Iterator>::difference_type* DistanceType(const Iterator&) {
	return static_cast<typename IteratorTraits<Iterator>::difference_type*>(0);
}

//萃取获得value_type(对象类型)
template <class Iterator>
inline typename IteratorTraits<Iterator>::value_type* ValueType(const Iterator&) {
	return static_cast<typename IteratorTraits<Iterator>::value_type*>(0);
}


//////////////////两种偏特化版本(指针与const指针)///////////////////
template <class T>
struct IteratorTraits<T*> {
  typedef RandomAccessIteratorTag iterator_category;
  typedef T                          value_type;
  typedef ptrdiff_t                  difference_type;
  typedef T*                         pointer;
  typedef T&                         reference;
};

template <class T>
struct IteratorTraits<const T*> {
  typedef RandomAccessIteratorTag iterator_category;
  typedef T                          value_type;
  typedef ptrdiff_t                  difference_type;
  typedef const T*                   pointer;
  typedef const T&                   reference;
};

#endif