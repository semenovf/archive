/**
 * @file   errorable_ext.hpp
 * @author wladt
 * @date   Jan 31, 2013 2:34:11 PM
 *
 * @brief
 * 		   on Mar 15, 2015 renamed to errorable_ext (extended)
 * 		   and placed into `pfs' namespace
 */

#ifndef __PFS_ERRORABLE_EXT_HPP__
#define __PFS_ERRORABLE_EXT_HPP__

#include <pfs/errorable.hpp>

//#ifdef PFS_CC_MSVC
//#	pragma warning(push)
//#	pragma warning(disable:4251)
//#endif

namespace pfs {
// Extended errorable class
//
class DLL_API errorable_ext : public errorable
{
public:
	void addSystemError (int errn, const string & caption);
	void logErrors ();
};

} // pfs

//#ifdef PFS_CC_MSVC
//#	pragma warning(pop)
//#endif

#endif /* __PFS_ERRORABLE_EXT_HPP__ */
