/*
 * rect.hpp
 *
 *  Created on: Mar 29, 2016
 *      Author: wladt
 */

#ifndef __PFS_RECT_HPP__
#define __PFS_RECT_HPP__

#include <pfs/details/rect.hpp>

namespace pfs {

typedef pfs::details::rect<int> rect_int;
typedef pfs::details::rect<float> rect_float;
typedef pfs::details::rect<double> rect_double;

}


#endif /* __PFS_RECT_HPP__ */
