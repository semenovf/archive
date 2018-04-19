/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iterator.hpp
 * Author: wladt
 *
 * Created on December 30, 2016, 11:00 AM
 */

#ifndef __PFS_CXX98_ITERATOR_HPP__
#define __PFS_CXX98_ITERATOR_HPP__

#include <iterator>
#include <pfs/types.hpp>

namespace pfs {

typedef std::input_iterator_tag   input_iterator_tag;
typedef std::output_iterator_tag  output_iterator_tag;
typedef std::forward_iterator_tag forward_iterator_tag;
typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
typedef std::random_access_iterator_tag random_access_iterator_tag;

template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template <typename T>
struct iterator_traits <T *>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef T         value_type;
    typedef ptrdiff_t difference_type;
    typedef T *       pointer;
    typedef T &       reference;
};

template <typename T>
struct iterator_traits<const T *>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef T         value_type;
    typedef ptrdiff_t difference_type;
    typedef const T * pointer;
    typedef const T & reference;
};
    
template< class InputIt>
inline typename pfs::iterator_traits<InputIt>::difference_type
distance (InputIt begin, InputIt end)
{
    return std::distance(begin, end);
}

template<typename InputIterator, typename Distance>
inline void
advance (InputIterator & it, Distance n)
{
    return std::advance(it, n);
}

} // pfs

#endif /* __PFS_CXX98_ITERATOR_HPP__ */

