/*
 * dl.hpp
 *
 *  Created on: 06.07.2012
 *      Author: wladt
 */
#ifndef __PFS_DL_HPP__
#define __PFS_DL_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */
#include <pfs/string.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/pluggable.hpp>
#include <pfs/error_code.hpp>
#include <pfs/fs/path.hpp>

#ifdef PFS_CC_MSVC
#	include <windows.h>
#	include <windef.h>
#else
#	include <dlfcn.h>
#endif

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API dynamic_library
{
public:
#ifdef PFS_CC_MSC
	typedef HMODULE handle;
	typedef FARPROC symbol;
#else
	typedef void * handle;
	typedef void * symbol;
#endif

private:
	struct shared_data
	{
		handle     _handle;
		fs::path   _path;   // contains path to dynamic library

		shared_data () : _handle(0) {}
		~shared_data ();
	};

	shared_ptr<shared_data> _d;

public:
	dynamic_library ()
		: _d(new shared_data)
	{};

	~dynamic_library ()
	{}

	handle native () const
	{
		return _d->_handle;
	}

	/**
	 * @brief Open (load) dynamic library (shared object).
	 *
	 * @param p path to dynamic library file (relative or absolute).
	 *
	 * @return @c true if dynamic library opened (loaded) successfully,
	 *         @c false if an error occurred.
	 */
	bool open (const fs::path & p, const fs::pathlist & searchdirs
			, error_code * ex = 0
			, string * extended_errstr = 0);

	symbol resolve (const char * symbol_name
			, error_code * ex = 0
			, string * extended_errstr = 0);

//	void close ();

//	pfs::pluggable * open_plugin (const string & name, const fs::path & path);
//
//	pfs::pluggable * open_plugin (const string & name);
//
//	bool close_plugin (const string & name, pfs::pluggable * pluggable);

	static fs::path build_filename (const string & name);
};

} // pfs

EXTERN_C_BEGIN
int DLL_API dl_only_for_testing_purpose (void);
EXTERN_C_END

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /*__PFS_DL_HPP__ */
