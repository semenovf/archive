/**
 * @file   errorable.hpp
 * @author wladt
 * @date   Jan 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __CWT_ERRORABLE_HPP__
#define __CWT_ERRORABLE_HPP__

#include <pfs/errorable.hpp>

//#ifdef PFS_CC_MSVC
//#	pragma warning(push)
//#	pragma warning(disable:4251)
//#endif

namespace cwt {

class DLL_API errorable : public pfs::errorable
{
public:
	void addSystemError (int errn, const pfs::string & caption);
	void logErrors ();
};

} // cwt

//#ifdef PFS_CC_MSVC
//#	pragma warning(pop)
//#endif

#endif /* __CWT_ERRORABLE_HPP__ */
