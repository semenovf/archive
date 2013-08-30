/**
 * @file   stack.hpp
 * @author wladt
 * @date   Feb 6, 2013 4:20:42 PM
 *
 * @brief
 */

#ifndef __CWT_STACK_HPP__
#define __CWT_STACK_HPP__

#include <cwt/vector.hpp>

CWT_NS_BEGIN

template <typename T>
class Stack : public Vector<T>
{
	typedef typename Vector<T> base_class;
public:
	Stack() {}
	Stack(const Stack & other) : base_class (dynamic_cast<const basr_class &>(other)) {}
	Stack &	   operator = (const Stack & other) { base_class::operator = (other); return *this; }

	T         pop();
	void      push(const T &t) {  { Vector<T>::append(t); } }
	T &       top();
	const T & top() const;
};

template<typename T>
T Stack<T>::pop()
{
	CWT_ASSERT(!this->isEmpty());
	Vector<T>::m_d.detach();
	T t = this->data()[this->size() -1];
	this->resize(this->size()-1); return t;
}

template<typename T>
T & Stack<T>::top()
{
	CWT_ASSERT(!this->isEmpty());
	Vector<T>::m_d.detach();
	return this->data()[this->size()-1];
}

template<typename T>
inline const T & Stack<T>::top() const
{
	CWT_ASSERT(!this->isEmpty());
	return this->data()[this->size()-1];
}

CWT_NS_END

#endif /* __CWT_STACK_HPP__ */
