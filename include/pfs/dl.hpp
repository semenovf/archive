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

class DLL_API dl
{
public:
#ifdef PFS_CC_MSC
	typedef HMODULE handle;
	typedef FARPROC symbol;
#else
	typedef void * handle;
	typedef void * symbol;
#endif

//	typedef pfs::map<string, handle>::type plugin_map_type;
//	typedef pfs::vector<fs::path>::type path_list_type;

private:
//	path_list_type  _search_paths;
//	plugin_map_type _plugins;

	handle _handle;
	string _path;   // contains path to dynamic library

private:
	static fs::path build_filename (const string & name);

public:
	dl () : _handle(0)
	{};

	~dl ()
	{
		close();
	}

	handle native () const
	{
		return _handle;
	}

	bool open (const fs::path & p);

	symbol resolve (const char * symbol_name, error_code * ex = 0);

	void close ();

//	pfs::pluggable * open_plugin (const string & name, const fs::path & path);
//
//	pfs::pluggable * open_plugin (const string & name);
//
//	bool close_plugin (const string & name, pfs::pluggable * pluggable);
//
//	void clear_search_path ()
//	{
//		_search_paths.clear();
//	}

//	void add_search_path (const fs::path & dir)
//	{
//		_search_paths.push_back(dir);
//	}

private:
	static fs::path search_file (const fs::path & libpath, const fs::pathlist searchpaths, error_code * ex = 0);
};

} // pfs

EXTERN_C_BEGIN
int DLL_API dl_only_for_testing_purpose (void);
EXTERN_C_END

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /*__PFS_DL_HPP__ */
