/*
 * constness.hpp
 *
 *  Created on: Sep 29, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_CONSTNESS_HPP__
#define __PFS_BITS_CONSTNESS_HPP__

namespace pfs {

template<typename Holder>
struct constness
{
	typedef typename Holder::pointer pointer;
	typedef typename Holder::reference reference;
};

template<typename Holder>
struct constness<const Holder>
{
	typedef typename Holder::const_pointer pointer;
	typedef typename Holder::const_reference reference;
};

} // pfs

#endif /* __PFS_BITS_CONSTNESS_HPP__ */
