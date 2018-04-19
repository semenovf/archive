/*
 * list.hpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_LIST_HPP__
#define __CWT_LIST_HPP__

#include <list>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

template <typename T>
class DLL_API List
{
	class Impl : public std::list<T>
	{
	public:
		typedef typename std::list<T>::iterator iterator;
		typedef typename std::list<T>::const_iterator const_iterator;
	public:
		Impl() : std::list<T>() {}
		Impl(const Impl & other);
	};

	CWT_PIMPL_COPYABLE(List);

public:
	typedef typename Impl::iterator iterator;
	typedef typename Impl::const_iterator const_iterator;

public:
    List ();

	void append(const T & value);
	void append(const T * value, size_t count);
	void append(const List<T> & other);
	void prepend(const T & value);
	void prepend(const T * value, size_t count);
	void prepend(const List<T> & other);

	bool isEmpty() const { return pimpl->empty(); }

	T &            first()         { CWT_ASSERT(pimpl->size() > 0); return pimpl->front(); }
	const T &      first() const   { CWT_ASSERT(pimpl->size() > 0); return pimpl->front(); }
	T &            last()          { CWT_ASSERT(pimpl->size() > 0); return pimpl->back(); }
	const T &      last() const    { CWT_ASSERT(pimpl->size() > 0); return pimpl->back(); }
	size_t         size() const    { return pimpl->size(); }
	size_t         length() const  { return size(); }
};

template <typename T>
inline List<T>::List () : pimpl(new List::Impl)
{}

template <typename T>
inline void List<T>::append(const T & value)
{
	detach();
	pimpl->push_back(value);
}

template <typename T>
void List<T>::append(const T * value, size_t count)
{
	if (count > 0) {
		detach();
		while(count-- > 0)
			pimpl->push_back(*value++);
	}
}

template <typename T>
inline void List<T>::append(const List<T> & other)
{
	detach();
	pimpl->insert(pimpl->end(), other.pimpl->begin(), other.pimpl.end());
}

template <typename T>
inline void List<T>::prepend(const T & value)
{
	detach();
	pimpl->push_front(value);
}

template <typename T>
void List<T>::prepend(const T * value, size_t count)
{
	if (count > 0) {
		detach();
		typename Impl::iterator it = pimpl->begin();
		while(count-- > 0)
			it = pimpl->insert(it, *value++);
	}
}

template <typename T>
inline void List<T>::prepend(const List<T> & other)
{
	detach();
	pimpl->insert(pimpl->begin(), other.pimpl->begin(), other.pimpl.end());
}


#ifdef __COMMENT__
template <class Key, class T>
bool Map<Key, T>::operator == (const Map<Key, T> & other) const
{
    if (size() != other.size())
        return false;
    if (m_d == other.m_d)
        return true;

    const_iterator it = begin();

    while (it != end()) {
        const Key & key = it->first();
        const_iterator it2 = other.find(key);

        if (it2 == other.end() || !(it2->first() == key))
        	return false;
        ++it;
    }
    return true;
}
#endif

template <typename T>
inline List<T>::Impl::Impl(const List::Impl & other)
	: std::list<T>()
{
	this->insert(std::list<T>::begin()
			, other.begin(), other.end());
}

CWT_NS_END

#endif /* __CWT_LIST_HPP__ */
