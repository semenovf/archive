/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 3:21 PM
 */

#ifndef __PFS_MPL_STDCXX_STRING_HPP__
#define __PFS_MPL_STDCXX_STRING_HPP__

namespace pfs {
namespace mpl {

template <>
struct string_traits<std::string>
{
    typedef std::string const &          const_impl_reference;
    typedef std::string::size_type       size_type;
    typedef std::string::value_type      value_type;
    typedef std::string::const_pointer   const_pointer;
    typedef std::string::const_iterator  const_iterator;
    typedef std::string::const_reverse_iterator const_reverse_iterator;

    typedef std::string data_type;
    
//    static size_type size (const_pointer);
//    int compare_n (const_pointer lhs, const_pointer rhs, size_type n);
};

//template <typename T>
//class basic_cxxstring;
//
//template <typename T>
//class basic_cxxstring<T> : public basic_string<T>
//{
//    typedef basic_string<T> basic_type;
//    typedef typename basic_type::data_type data_type;
//    
//public:    
//    typedef typename basic_type::traits                 traits;
//    typedef typename basic_type::const_impl_reference   const_impl_reference;
//    typedef typename basic_type::size_type              size_type;
//    typedef typename basic_type::value_type             value_type;
//    typedef typename basic_type::const_pointer          const_pointer;
//    typedef typename basic_type::const_iterator         const_iterator;
//    typedef typename basic_type::const_reverse_iterator const_reverse_iterator;
//    
//protected:
//    basic_cxxstring (const_iterator begin, const_iterator end)
//    {
//        this->_d.begin = begin;
//        this->_d.end = end;
//    }
//    
//public:
//    virtual size_type size () const
//    {
//        return this->_d.end - this->_d.begin;
//    }
//
//    virtual const_iterator begin () const
//    {
//        return this->_d.begin;
//    }
//
//    virtual const_iterator end () const
//    {
//        return this->_d.end;
//    }
//
//    virtual const_reverse_iterator rbegin () const
//    {
//        return const_reverse_iterator(this->_d.end);
//    }
//
//    virtual const_reverse_iterator rend () const
//    {
//        return const_reverse_iterator(this->_d.begin);
//    }
//    
//    virtual int compare (basic_type const & rhs, size_type n) const;
//};
//
//template <typename T>
//class string<std::basic_string<T> > : public basic_cxxstring<std::basic_string<T> >
//{
//    typedef basic_cstring<T *> basic_type;
//    
//public:    
//    typedef typename basic_type::traits                 traits;
//    typedef typename basic_type::const_impl_reference   const_impl_reference;
//    typedef typename basic_type::const_pointer          const_pointer;
//    typedef typename basic_type::const_iterator         const_iterator;
//
//public:
//    string ()
//        : basic_type(0, 0)
//    {}
//    
//    string (const_impl_reference rhs)
//        : basic_type(rhs, rhs + traits::size(rhs))
//    {}
//    
//    string (const_iterator begin, const_iterator end)
//        : basic_type(begin, end)
//    {}
//    
//    string (string const & rhs)
//        : basic_type(rhs._d.begin, rhs._d.end)
//    {}
//    
//    string & operator = (const_pointer rhs)
//    {
//        this->_d.begin = rhs;
//        this->_d.end = rhs + traits::size(rhs);
//        return *this;
//    }
//
////	bool ends_with (string const & rhs) const
////    {
////        return pfs::mpl::ends_with(this->begin(), this->end()
////                , rhs.begin(), rhs.end());
////    }
//};

//template <>
//struct const_string_traits<std::string>
//{
//    typedef std::string const &                 const_impl_reference;
//    typedef std::string::size_type              size_type;
//    typedef std::string::value_type             value_type;
//    typedef std::string::const_pointer          const_pointer;
//    typedef std::string::const_iterator         const_iterator;
//    typedef std::string::const_reverse_iterator const_reverse_iterator;
//
//    struct data_type 
//    {
//        std::string s;
//    };
//};
//
//template <>
//inline const_string<std::string>::
//const_string ()
//{}
//
//template <>
//inline const_string<std::string>::
//const_string (const_impl_reference s)
//{
//    _d.s = std::string(s);
//}
//
//template <>
//inline const_string<std::string>::
//const_string (const_iterator begin, const_iterator end)
//{
//    _d.s = std::string(begin, end);
//}
//
//template <>
//inline typename const_string<std::string>::const_iterator
//const_string<std::string>::begin () const
//{
//    return _d.s.begin();
//}
//
//template <>
//inline typename const_string<std::string>::const_iterator
//const_string<std::string>::end () const
//{
//    return _d.s.end();
//}
//
//template <>
//inline typename const_string<std::string>::const_reverse_iterator
//const_string<std::string>::rbegin () const
//{
//    return _d.s.rbegin();
//}
//
//template <>
//inline typename const_string<std::string>::const_reverse_iterator
//const_string<std::string>::rend () const
//{
//    return _d.s.rend();
//}
//
//template <>
//inline typename const_string<std::string>::const_pointer 
//const_string<std::string>::data () const
//{
//    return _d.s.data();
//}
//
//template <>
//inline typename const_string<std::string>::size_type 
//const_string<std::string>::size () const
//{
//    return _d.s.size();
//}
//
//template <>
//inline typename const_string<std::string>::size_type
//const_string<std::string>::length () const
//{
//    return this->size();
//}
//
//template <>
//inline int 
//const_string<std::string>::compare (const_string const & rhs) const
//{
//    return _d.s.compare(rhs._d.s);
//}

}} // pfs::mpl

#endif /* __PFS_MPL_STDCXX_STRING_HPP__ */