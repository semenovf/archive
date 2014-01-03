/**
 * @file   fs.hpp
 * @author wladt
 * @date   Feb 15, 2013 9:02:52 AM
 *
 * @brief
 */

#ifndef __CWT_FILESYSTEM_HPP__
#define __CWT_FS_HPP__

#include <pfs/string.hpp>
#include <cwt/errorable.hpp>

namespace cwt {

class fs : public errorable
{
public:
	fs () : errorable() {}
	pfs::ucchar separator ();
	bool isAbsolute (const pfs::string & path);
	bool isRelative (const pfs::string & path) { return !isAbsolute(path); }
	bool exists (const pfs::string & path);
	bool rename (const pfs::string & from, const pfs::string & to);
	bool remove (const pfs::string & path);
	bool unlink (const pfs::string & path) { return remove(path); }
	bool simpleBackup (const pfs::string & path);
};

} // cwt

#endif /* __CWT_FILESYSTEM_HPP__ */
