/*
 * size.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_DIMENSION_HPP__
#define __PFS_DIMENSION_HPP__

namespace pfs {

template <typename T, T ZeroValue = T(0)>
class dimension
{
private:
    T _w, _h;

public:
    dimension ()
        : _w(ZeroValue), _h(ZeroValue)
    {}

    dimension (T const & w, T const & h)
    	: _w(w), _h(h)
    {}

    dimension (dimension const & other)
        : _w(other._w), _h(other._h)
    {}

    dimension & operator = (dimension const & other)
    {
        _w = other._w;
        _h = other._h;
        return *this;
    }

    T width () const { return _w; }
    T height () const { return _h; }

    void set_width (T const & w) { _w = w; }
    void set_height (T const & h) { _h = h; }
    
    template <typename T1, T1 ZeroValue1>
    friend bool operator == (dimension<T1, ZeroValue1> const & lhs, dimension<T1, ZeroValue1> const & rhs);
    
    template <typename T1, T1 ZeroValue1>
    friend bool operator != (dimension<T1, ZeroValue1> const & lhs, dimension<T1, ZeroValue1> const & rhs);
};

template <typename T, T ZeroValue>
inline bool operator == (dimension<T, ZeroValue> const & lhs, dimension<T, ZeroValue> const & rhs)
{
    return lhs._w == rhs._w && lhs._h == rhs._h;
}

template <typename T, T ZeroValue>
inline bool operator != (dimension<T, ZeroValue> const & lhs, dimension<T, ZeroValue> const & rhs)
{
    return ! operator == (lhs, rhs);
}

} // pfs

#endif /* __PFS_DIMENSION_HPP__ */
