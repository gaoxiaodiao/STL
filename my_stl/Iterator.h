/*
*�ļ�˵��:����������
*����:��С��
*����:2017-07-20
*���ɿ�������:Microsoft Visual Studio 2010 
*/
#ifndef __ITERATOR_H__
#define __ITERATOR_H__
//���ֵ�������ǩ(����������ȡ)
struct InputIteratorTag{};												//ֻ��������
struct OutputIteratorTag{};												//ֻд������
struct ForwardIteratorTag:public InputIteratorTag {};					//ǰ�������
struct BidirectionalIteratorTag : public ForwardIteratorTag {};			//���������
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};	//������ʵ�����

//////////////////////////���ֵ������Ķ��忪ʼ//////////////////////////
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
//////////////////////////���ֵ������Ķ������//////////////////////////



////////////////////����������ȡ///////////////////////////
template <class Iterator>
struct IteratorTraits {
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
};

//��ȡ���iterator_category(����������)
template <class Iterator>
inline typename IteratorTraits<Iterator>::iterator_category IteratorCategory(const Iterator&) {
	typedef typename IteratorTraits<Iterator>::iterator_category category;
	return category();
}
//��ȡ���difference_type(����������֮��ľ���)
template <class Iterator>
inline typename IteratorTraits<Iterator>::difference_type* DistanceType(const Iterator&) {
	return static_cast<typename IteratorTraits<Iterator>::difference_type*>(0);
}

//��ȡ���value_type(��������)
template <class Iterator>
inline typename IteratorTraits<Iterator>::value_type* ValueType(const Iterator&) {
	return static_cast<typename IteratorTraits<Iterator>::value_type*>(0);
}


//////////////////����ƫ�ػ��汾(ָ����constָ��)///////////////////
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