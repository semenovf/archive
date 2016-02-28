/*
 * functor_map.hpp
 *
 *  Created on: Nov 26, 2015
 *      Author: wladt
 */

#ifndef __PFS_FUNCTOR_MAP_HPP__
#define __PFS_FUNCTOR_MAP_HPP__

#include <pfs/functor.hpp>
#include <pfs/mutex.hpp>
#include <pfs/map.hpp>

namespace pfs {

template <typename Key, typename Return, typename Mutex = pfs::fake_mutex>
class functor_map_base
{
public:
	typedef Key    key_type;
	typedef Return return_type;
	typedef functor_base<Return> functor_base_type;
	typedef functor_base_type * value_type;
	typedef typename pfs::map<key_type, value_type> map_type;
	typedef typename map_type::iterator iterator;
	typedef typename map_type::const_iterator const_iterator;

protected:
	Mutex    _mutex;
	map_type _m;

protected:
	void erase (iterator it)
	{
		delete it->second;
		_m.erase(it);
	}

public:
	functor_map_base ()	{}

	virtual ~functor_map_base ()
	{
		clear();
	}

	bool empty () const
	{
		return _m.size() == 0;
	}

	size_t count () const
	{
		return _m.size();
	}

	size_t size () const
	{
		return _m.size();
	}

	bool contains (const key_type & key) const
	{
		return _m.find(key) != _m.end();
	}

	void insert (const key_type key, return_type (* f) ())
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor0<return_type>(f)));
	}

	template <typename Class>
	bool insert_method (const key_type key, Class * p, return_type (Class::* f) ())
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method0<Class, return_type>(p, f)));
	}

	template <typename Arg1>
	void insert (const key_type key, return_type (*f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor1<return_type, Arg1>(f, a1)));
	}

	template <typename Class, typename Arg1>
	void insert_method (const key_type key, Class * p, return_type (Class::*f) (Arg1), Arg1 a1)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method1<Class, return_type, Arg1>(p, f, a1)));
	}

	template <typename Arg1, typename Arg2>
	void insert (const key_type key, return_type (*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor2<return_type, Arg1, Arg2>(f, a1, a2)));
	}

	template <typename Class, typename Arg1, typename Arg2>
	void insert_method (const key_type key, Class * p, return_type (Class::*f) (Arg1, Arg2), Arg1 a1, Arg2 a2)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method2<Class, return_type, Arg1, Arg2>(p, f, a1, a2)));
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	void insert (const key_type key, return_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor3<return_type, Arg1, Arg2, Arg3>(f, a1, a2, a3)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3>
	void insert_method (const key_type key, Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method3<Class, return_type, Arg1, Arg2, Arg3>(p, f, a1, a2, a3)));
	}

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void insert (const key_type key, return_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor4<return_type, Arg1, Arg2, Arg3, Arg4>(f, a1, a2, a3, a4)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	void insert_method (const key_type key, Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method4<Class, return_type, Arg1, Arg2, Arg3, Arg4>(p, f, a1, a2, a3, a4)));
	}

	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void insert (const key_type key, return_type (*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor5<return_type, Arg1, Arg2, Arg3, Arg4, Arg5>(f, a1, a2, a3, a4, a5)));
	}

	template <typename Class, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	void insert_method (const key_type key, Class * p, return_type (Class::*f) (Arg1, Arg2, Arg3), Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5)
	{
		lock_guard<Mutex> locker(_mutex);
		_m.insert(std::pair<key_type, value_type>(key, new functor_method5<Class, return_type, Arg1, Arg2, Arg3, Arg4, Arg5>(p, f, a1, a2, a3, a4, a5)));
	}

	void clear ()
	{
		lock_guard<Mutex> locker(_mutex);
		iterator it = _m.begin();

		while (it != _m.end()) {
			erase(it);
			++it;
		}
	}

	void erase (const key_type & key)
	{
		unique_lock<Mutex> locker(_mutex);
		iterator it = _m.find(key);

		while (it != _m.end()) {
			erase(it);
			it = _m.find(key);
		}
	}
};

template <typename Key, typename Return, typename Mutex = pfs::fake_mutex>
class functor_map : public functor_map_base<Key, Return, Mutex>
{
public:
	typedef functor_map_base<Key, Return, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::functor_base_type functor_base_type;
	typedef typename base_class::map_type map_type;
	typedef typename base_class::key_type key_type;
	typedef typename base_class::value_type value_type;

	functor_map () : base_class() {}

	return_type call (const key_type & key);
	return_type call_all ();

	return_type call_and_erase (const key_type & key)
	{
		return_type r = call(key);
		this->erase(key);
		return r;
	}

	return_type call_all_and_clear ()
	{
		return_type r = call_all();
		this->clear();
		return r;
	}
};

template <typename Key, typename Mutex>
class functor_map<Key, void, Mutex> : public functor_map_base<Key, void, Mutex>
{
public:
	typedef functor_map_base<Key, void, Mutex> base_class;
	typedef typename base_class::return_type return_type;
	typedef typename base_class::functor_base_type functor_base_type;
	typedef typename base_class::map_type map_type;
	typedef typename base_class::key_type key_type;
	typedef typename base_class::value_type value_type;

	functor_map () : base_class() {}

	return_type call (const key_type & key);
	return_type call_all ();

	return_type call_and_erase (const key_type & key)
	{
		call(key);
		this->erase(key);
	}

	return_type call_all_and_clear ()
	{
		call_all();
		this->_m.clear();
	}
};


template <typename Key, typename Return, typename Mutex>
typename functor_map<Key, Return, Mutex>::return_type
	functor_map<Key, Return, Mutex>::call (const key_type & key)
{
	lock_guard<Mutex> locker(this->_mutex);

	typename map_type::iterator it = this->_m.find(key);
	return_type r;

	if (it != this->_m.end()) {
		r = (*it->second)();
	}

	return r;
}

template <typename Key, typename Return, typename Mutex>
typename functor_map<Key, Return, Mutex>::return_type
	functor_map<Key, Return, Mutex>::call_all ()
{
	lock_guard<Mutex> locker(this->_mutex);
	return_type r;

	typename map_type::iterator it = this->_m.begin();
	typename map_type::iterator itEnd = this->_m.end();

	while (it != itEnd) {
		r = (*it->second)();
		++it;
	}

	return r;
}

template <typename Key, typename Mutex>
typename functor_map<Key, void, Mutex>::return_type
	functor_map<Key, void, Mutex>::call (const key_type & key)
{
	lock_guard<Mutex> locker(this->_mutex);
	typename map_type::iterator it = this->_m.find(key);

	if (it != this->_m.end()) {
		(*it->second)();
	}
}

template <typename Key, typename Mutex>
typename functor_map<Key, void, Mutex>::return_type
	functor_map<Key, void, Mutex>::call_all ()
{
	lock_guard<Mutex> locker(this->_mutex);

	typename map_type::iterator it = this->_m.begin();
	typename map_type::iterator itEnd = this->_m.end();

	while (it != itEnd) {
		(*it->second)();
		++it;
	}
}

} // pfs

#endif /* __PFS_FUNCTOR_QUEUE_HPP__ */
