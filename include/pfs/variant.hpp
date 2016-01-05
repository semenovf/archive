/*
 * variant.hpp
 *
 *  Created on: Feb 2, 2015
 *      Author: wladt
 */

#ifndef __PFS_VARIANT_HPP__
#define __PFS_VARIANT_HPP__

#include <pfs.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/utility.hpp>
#include <cstring>
#include <typeinfo>

namespace pfs {

template <typename T, size_t Size = 0>
struct max_alignof
{
	enum { result = alignof(T) > Size ? alignof(T) : Size };
};

inline uint32_t variant_hash_code (const char * name)
{
	return pfs::hash32(name, strlen(name), static_cast<unsigned int>(0xc70f6907UL));
}

template <typename T>
void variant_destroyer (void * p)
{
	reinterpret_cast<T*>(p)->~T();
}

template <typename T>
void variant_copier (void * dest, const void * src)
{
	new (dest) T(*reinterpret_cast<const T*>(src));
}

template <typename T1 = char, typename T2 = char, typename T3 = char
		, typename T4 = char, typename T5 = char, typename T6 = char
		, typename T7 = char, typename T8 = char, typename T9 = char
		, typename T10 = char, typename T11 = char, typename T12 = char>
class variant
{
	enum {
	MaxSize =
		  max_sizeof<T1, max_sizeof<T2, max_sizeof<T3, max_sizeof<T4
		, max_sizeof<T5, max_sizeof<T6, max_sizeof<T7, max_sizeof<T8
		, max_sizeof<T9, max_sizeof<T10, max_sizeof<T11, max_sizeof<T12
		>
		::result >::result >::result >::result >
		::result >::result >::result >::result >
		::result >::result >::result >::result
	,
	MaxAlign =
		  max_alignof<T1, max_alignof<T2, max_alignof<T3, max_alignof<T4
		, max_alignof<T5, max_alignof<T6, max_alignof<T7, max_alignof<T8
		, max_alignof<T9, max_alignof<T10, max_alignof<T11, max_alignof<T12
		>
		::result >::result >::result >::result >
		::result >::result >::result >::result >
		::result >::result >::result >::result
	};

	typedef uint32_t hash_type;
	typedef typename pfs::aligned_storage<MaxSize, MaxAlign>::type data_type;

	hash_type _hashCode;
	data_type _data;

	void (* _destroy) (void *);
	void (* _copy) (void * dest, const void * src);

private:
	static hash_type invalidHashCode ()
	{
		return variant_hash_code(typeid(void).name());
	}

	void destroy ();

public:
	variant ()
		: _hashCode(invalidHashCode())
		, _destroy(0)
		, _copy(0)
	{}

	variant (const variant & other)
		: _hashCode(other._hashCode)
		, _destroy(other._destroy)
		, _copy(other._copy)
	{
		if (_copy)
			_copy(& _data, & other._data);
	}

	template <typename T>
	explicit variant (const T & v)
		: _hashCode(invalidHashCode())
		, _destroy(0)
		, _copy(0)
	{
		set<T>(v);
	}

	~variant () { destroy(); }

	bool isValid () const
	{
		return _hashCode != invalidHashCode();
	}

	variant & operator = (const variant & other);

	template <typename T>
	variant & operator = (const T & v)
	{
		set<T>(v);
		return *this;
	}

	template <typename T>
	bool is () const
	{
		return variant_hash_code(typeid(T).name()) == _hashCode;
	}

	template <typename T>
	void set (const T & v);

	template <typename T>
	T & get ()
	{
		PFS_ASSERT_BAD_CAST(is<T>());
		return *reinterpret_cast<T*>(& _data);
	}

	template <typename T>
	const T & get () const
	{
		PFS_ASSERT_BAD_CAST(is<T>());
		return *reinterpret_cast<const T *>(& _data);
	}
};

template <typename T1, typename T2, typename T3
		, typename T4, typename T5, typename T6
		, typename T7, typename T8, typename T9
		, typename T10, typename T11, typename T12>
void variant<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::destroy ()
{
	if (_destroy)
		_destroy(& _data);
	_hashCode = invalidHashCode();
	_destroy = 0;
	_copy = 0;
}

template <typename T1, typename T2, typename T3
		, typename T4, typename T5, typename T6
		, typename T7, typename T8, typename T9
		, typename T10, typename T11, typename T12>
variant<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &
variant<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::operator = (
		const variant<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> & other)
{
	destroy();
	_hashCode = other._hashCode;
	_destroy = other._destroy;
	_copy = other._copy;
	if (_copy)
		_copy(& _data, & other._data);
	return *this;
}

template <typename T1, typename T2, typename T3
		, typename T4, typename T5, typename T6
		, typename T7, typename T8, typename T9
		, typename T10, typename T11, typename T12>
template <typename T>
void variant<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::set (const T & v)
{
	if (isValid())
		destroy();
	_destroy = variant_destroyer<T>;
	_copy    = variant_copier<T>;
	new (& _data) T(v);
	_hashCode = variant_hash_code(typeid(T).name());
}

} // pfs

#endif /* __PFS_VARIANT_HPP__ */
