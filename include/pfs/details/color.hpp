/*
 * color.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_DETAILS_COLOR_HPP__
#define __PFS_DETAILS_COLOR_HPP__

namespace pfs { namespace details {

struct rgb_color_tag
{
	int _r, _g, _b, _a;
};

struct hsv_color_tag
{
	int _h, _s, _v, _a;
};

struct hsl_color_tag
{
	int _h, _s, _l, _a;
};

struct cmyk_color_tag
{
	int _c, _m, _y, _k;
};

template <typename ColorTag, typename String>
class color
{
private:
	ColorTag _d;
};

}}

#endif /* __PFS_DETAILS_COLOR_HPP__ */
