/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vector.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 11:44 AM
 */

#ifndef __PFS_MPL_VECTOR_HPP__
#define __PFS_MPL_VECTOR_HPP__

namespace pfs {
namespace mpl {

template <typename T>
struct vector_traits
{
//    typedef T const &                          const_impl_reference;
    typedef typename T::size_type              size_type;
    typedef typename T::value_type             value_type;
//    typedef typename T::const_pointer          const_pointer;
//    typedef typename T::const_iterator         const_iterator;
//    typedef typename T::const_reverse_iterator const_reverse_iterator;
    typedef T data_type;
};

template <typename T>
class basic_vector;

namespace details {
    
template <typename T>
class basic_vector 
{
//public:
//    typedef string_traits<T>                             traits_type;
//    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
//    typedef typename traits_type::const_pointer          const_pointer;
//    typedef typename traits_type::const_iterator         const_iterator;
//    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;

public:
    data_type _d;

public:
    basic_vector ()
    {}

    basic_vector (data_type const & d)
        : _d(d)
    {}
};

} // details

template <typename T>
class vector : public basic_vector<T>
{
    typedef basic_vector<T> base_type;
    
public:
	explicit vector ()
		: base_type()
	{}

	vector (size_type count, const T & value)
		: base_type(count, value)
	{}

	explicit vector (size_type count)
		: base_type(count, T())
	{}

	template <class InputIt>
	vector (InputIt first, InputIt last)
		: base_type(first, last)
	{}

	vector (vector const & x)
        : base_type(x)
	{}

#if __cplusplus >= 201103L
	vector (vector && other)
		: base_type(other)
	{}

	vector (std::initializer_list<T> init)
		: base_type(init)
	{}
#endif

//	const_iterator cbegin () const
//	{
//		return this->begin();
//	}
//
//	const_iterator cend () const
//	{
//		return this->end();
//	}
//
//	reference first ()
//	{
//		return this->front();
//	}
//
//	const_reference first () const
//	{
//		return this->front();
//	}
//
//	reference last ()
//	{
//		return this->back();
//	}
//
//	const_reference last () const
//	{
//		return this->back();
//	}
};

}}

#endif /* __PFS_MPL_VECTOR_HPP__ */

