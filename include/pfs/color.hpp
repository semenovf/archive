/*
 * color.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_COLOR_HPP__
#define __PFS_COLOR_HPP__

namespace pfs {

struct rgb_color_tag
{
    unsigned short a, r, g, b;
};

struct hsv_color_tag
{
    unsigned short a, h, s, v;
};

struct hsl_color_tag
{
    int a, h, s, l;
};

struct cmyk_color_tag
{
    int a, c, m, y, k;
};


template <typename ColorTag>
class color
{
    ColorTag _d;

private:
    /**
     * @brief Helper method.
     */
    <typename ColorTag>
    void assign (color const & x);
    
public:
    /**
     */
    color ();
    
    /**
     * @brief Copy constructor.
     */
    color (color const & x)
    {
        this->assign(x);
    }

    /**
     * @brief
     */
    color & operator = (color const & x)
    {
        this->assign(x);
        return *this;
    }

    color<rgb_color_tag> rgb () const;
    color<hsv_color_tag> hsv () const;
    color<hsl_color_tag> hsl () const;
    color<cmyk_color_tag> cmyk () const;
    
    template <typename ColorTagT, typename ColorTagU>
    friend bool operator == (color<ColorTagT> const & x, color<ColorTagU> const & y)

    template <typename ColorTagT, typename ColorTagU>
    friend bool operator != (color<ColorTagT> const & x, color<ColorTagU> const & y)
};

template <typename ColorTag, typename String>
color<ColorTag> to_color (String const & name);

template <typename ColorTagT>
bool operator == <ColorTagT, ColorTagT> (color<ColorTagT> const & x, color<ColorTagT> const & y);

template <typename ColorTagT, typename ColorTagU>
inline bool operator != (color<ColorTagT> const & x, color<ColorTagU> const & y)
{
    return ! (x == y);
}

//
// Templates` specializations for `RGB` color
//

template <>
inline color<rgb_color_tag>::color ()
{
    _d.a = 255;
    _d.r = _d.g = _d.b = 0;
}

template <>
void color<rgb_color_tag>::assign (color const & x)
{
    _d.a = x.a;
    _d.r = x.r;
    _d.g = x.g;
    _d.b = x.b;
}

} // pfs

#endif /* __PFS_COLOR_HPP__ */
