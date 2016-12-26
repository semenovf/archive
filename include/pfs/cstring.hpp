/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cstring.hpp
 * Author: wladt
 *
 * Created on December 26, 2016, 1:20 PM
 */

#ifndef __PFS_CSTRING_HPP__
#define __PFS_CSTRING_HPP__

#include <cstring>
#include <pfs.hpp>
#include <pfs/algo/find.hpp>

namespace pfs {

template <typename T>
class cstring_wrapper
{
public:    
    typedef size_t size_type;
    typedef T      value_type;
    typedef T *    iterator;
    typedef T *    reverse_iterator;

private:
    iterator _begin;
    iterator _end;
    
public:
    explicit cstring_wrapper (T * s)
        : _begin(s)
        , _end(begin + std::strlen(s))
    {
        PFS_ASSERT_INVALID_ARGUMENT(s);
    }

    iterator begin ()
    {
    	return _begin;
    }

    iterator end ()
    {
    	return _end;
    }

    reverse_iterator rbegin ()
    {
    	return _end - 1;
    }

    reverse_iterator rend ()
    {
    	return _begin - 1;
    }

	T * data () const
	{
		return _begin;
	}

    const T * c_str () const
    {
    	return _begin;
    }

	size_type size () const
	{
		return _end - _begin;
	}

    size_type length () const
    {
    	return this->size();
    }

    bool empty () const
    {
    	return size() == 0;
    }

	int compare (cstring_wrapper const & lhs) const
	{
        size_type n = this->size();
        if (lhs->size() < n)
            n = lhs->size();
		return std::strncmp(_begin, lhs._begin, n);
	}
    
    bool starts_with (cstring_wrapper const & needle) const
    {
        return starts_with(_begin, _end, needle._begin, needle._end);
    }

	bool ends_with (cstring_wrapper const & needle) const
    {
        return ends_with(_begin, _end, needle._begin, needle._end);
    }

	cstring_wrapper substr (iterator begin, iterator end) const
    {
        PFS_ASSERT_INVALID_ARGUMENT(begin <= end);
        PFS_ASSERT_INVALID_ARGUMENT(begin >= _begin);
        PFS_ASSERT_INVALID_ARGUMENT(end < _end);
        cstring_wrapper(begin, end);
    }
    
    cstring_wrapper substr (iterator begin) const
	{
		return substr(begin, _end);
	}

    cstring_wrapper substr (size_type pos, size_type count) const
    {
        return substr(_begin + pos, _begin + pos + count);
    }
    
	cstring_wrapper substr (size_t pos) const
	{
		return substr(pos, max_value<size_type>());
	}

	cstring_wrapper mid (size_t pos, size_t count) const
	{
		return substr(pos, count);
	}

	cstring_wrapper left (size_t count) const
	{
		return substr(0, count);
	}

	cstring_wrapper right (size_t count) const
	{
		return substr(length() - count, count);
	}
};

} // pfs

#endif /* __PFS_CSTRING_HPP__ */

