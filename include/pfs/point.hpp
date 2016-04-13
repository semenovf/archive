/*
 * point.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_POINT_HPP__
#define __PFS_POINT_HPP__

namespace pfs {

template <typename T, T ZeroValue = T(0)>
class point
{
private:
    T _x, _y;

public:
    point ()
        : _x(ZeroValue), _y(ZeroValue)
    {}

    point (T const & x, T const & y)
        : _x(x), _y(y)
    {}

    point (point const & other)
        : _x(other._x), _y(other._y)
    {}

    point operator = (point const & other)
    {
        _x = other._x;
        _y = other._y;
        return *this;
    }

    T x () const { return _x; }
    T y () const { return _y; }

    void set_x (T x) { _x = x; }
    void set_y (T y) { _y = y; }
};

} // pfs

#endif /* __PFS_POINT_HPP__ */
