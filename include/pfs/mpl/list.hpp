/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   list.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 12:08 PM
 */

#ifndef __PFS_MPL_LIST_HPP__
#define __PFS_MPL_LIST_HPP__

namespace pfs {
namespace mpl {

template <typename T, template <typename> class ListT>
struct list_traits
{
    typedef ListT<T> const &                          const_impl_reference;
    typedef typename ListT<T>::size_type              size_type;
    typedef typename ListT<T>::difference_type        difference_type;
    typedef typename ListT<T>::value_type             value_type;
    typedef typename ListT<T>::pointer                pointer;
    typedef typename ListT<T>::const_pointer          const_pointer;
    typedef typename ListT<T>::reference              reference;
    typedef typename ListT<T>::const_reference        const_reference;
    typedef typename ListT<T>::iterator               iterator;
    typedef typename ListT<T>::const_iterator         const_iterator;
    typedef typename ListT<T>::reverse_iterator       reverse_iterator;
    typedef typename ListT<T>::const_reverse_iterator const_reverse_iterator;
    
    typedef ListT<T> data_type;
};

template <typename T, template <typename> class ListT>
class basic_list;

namespace details {
    
template <typename T, template <typename> class ListT>
class basic_list 
{
public:
    typedef list_traits<T, ListT>                        traits_type;
    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::pointer                pointer;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::reference              reference;
    typedef typename traits_type::const_reference        const_reference;
    typedef typename traits_type::iterator               iterator;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::reverse_iterator       reverse_iterator;
    typedef typename traits_type::difference_type        difference_type;
    typedef typename traits_type::data_type              data_type;

public:
    data_type _d;

public:
    basic_list ()
    {}

    basic_list (data_type const & d)
        : _d(d)
    {}
    
    basic_list (size_type count, value_type const & value)
    	: _d(count, value)
	{}

	explicit basic_list (size_type count)
		: _d(count, value_type())
	{}
};

} // details

template <typename T, template <typename> class ListT>
class list : public basic_list<T, ListT>
{
    typedef basic_list<T, ListT> base_type;

public:    
//    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
//    typedef typename traits_type::pointer                pointer;
//    typedef typename traits_type::const_pointer          const_pointer;
//    typedef typename traits_type::reference              reference;
//    typedef typename traits_type::const_reference        const_reference;
//    typedef typename traits_type::iterator               iterator;
//    typedef typename traits_type::const_iterator         const_iterator;
//    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
//    typedef typename traits_type::reverse_iterator       reverse_iterator;
//    typedef typename traits_type::difference_type        difference_type;
//    typedef typename traits_type::data_type              data_type;
   
public:
	explicit list ()
		: base_type()
	{}

	list (size_type count, value_type const & value)
		: base_type(count, value)
	{}

	explicit list (size_type count)
		: base_type(count, T())
	{}

//	template <class InputIt>
//	list (InputIt first, InputIt last)
//		: base_type(first, last)
//	{}

	list (list const & rhs)
        : base_type(rhs)
	{}
    
    list & operator = (list const & rhs)
    {
        if (this != & rhs)
            base_type::operator = (rhs);
        return *this;
    }
};

}}

#endif /* __PFS_MPL_LIST_HPP__ */

