/*
 * timer.hpp
 *
 *  Created on: Sep 17, 2015
 *      Author: wladt
 */

#ifndef __PFS_TIMER_HPP__
#define __PFS_TIMER_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/sigslot.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct timer_impl;

class DLL_API timer : public nullable<timer_impl>, public has_slots<>
{
	typedef nullable<timer_impl> base_class;

public:
	timer () : base_class() {}

public: // slots
	void start (int msec);
	void start ();
	void stop ();

public: // signals
	signal0<> emitTimeout;
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_TIMER_HPP__ */
