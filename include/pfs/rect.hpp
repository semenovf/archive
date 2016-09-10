/*
 * rect.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_RECT_HPP__
#define __PFS_RECT_HPP__

#include <pfs/point.hpp>
#include <pfs/dimension.hpp>

namespace pfs {

template <typename T, T ZeroValue = T(0)>
class rect
{
public:
    typedef point<T, ZeroValue> point_type;
    typedef dimension<T, ZeroValue> dimension_type;
	
private:
    point_type     _p;
    dimension_type _d;

public:
    rect ()
        : _p(ZeroValue, ZeroValue), _d(ZeroValue, ZeroValue)
    {}

    rect (T const & x, T const & y, T const & w, T const & h)
        : _p(x, y), _d(w, h)
    {}

    rect (point_type const & p, dimension_type & d)
        : _p(p), _d(d)
    {}

    rect (rect const & other)
        : _p(other._p), _d(other._d)
    {}

    rect & operator = (rect const & other)
    {
        _p = other._p;
        _d = other._d;
        return *this;
    }

    T x ()      const { return _p.x(); }
    T y ()      const { return _p.y(); }
    T left ()   const { return _p.x(); }
    T top ()    const { return _p.y(); }
    T width ()  const { return _d.width(); }
    T height () const { return _d.height(); }

    dimension_type dim () const { return _d; }
    point_type pos () const { return _p; }

    void set_x (T const & x)    { _p.set_x(x); }
    void set_y (T const & y)    { _p.set_y(y); }
    void set_left (T const & x) { _p.set_x(x); }
    void set_top (T const & y)  { _p.set_y(y); }

    void set_width (T const & w)  { _d.set_width(w); }
    void set_height (T const & h) { _d.set_height(h); }
    void set_dim (T const & w, T const & h) { _d = dimension_type(w, h); }
    void set_dim (dimension_type const & d) { _d = d; }

    void move_to (T const & x, T const & y) { _p = point_type(x, y); }
    void move_to (point_type const & p)     { _p = p; }

    void move_by (T const & dx, T const & dy)
    {
        _p = point_type(_p.x() + dx, _p.y() + dy);
    }

    template <typename U>
    void move_by (U const & dx, U const & dy)
    {
        _p = point_type(_p.x() + T(dx), _p.y() + T(dy));
    }
};

} // pfs

#endif /* __PFS_RECT_HPP__ */
