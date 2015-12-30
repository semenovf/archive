#include "pfs/dl.hpp"
#include "pfs/fs/path.hpp"

namespace pfs {

//dl & dl::getDL ()
//{
//	static pfs::dl dl;
//	return dl;
//}

/**
 * @note  internal
 *
 * @brief Searches file in directories added by addSearchPath()
 *        and returns absolute path.
 *
 * @param filename File name to search. May be absolute or relative.
 * @return Absolute file path or empty path if @a filename is empty
 *         or file not found in list of directories specified for search.
 */
fs::path dl::search_file (const fs::path & libpath, const fs::pathlist searchpaths, error_code * ex)
{
	if (libpath.empty())
		return fs::path();

	if (fs::exists(libpath)) {
		if (libpath.is_absolute())
			return libpath;

		/*
		 * If libpath is not an absolute path
		 * then prepend current directory and return result
		 */
		error_code ex1;
		fs::path curr_dir = fs::current_directory(& ex1);

		if (ex1) {
			if (ex)
				*ex = ex1;
			return fs::path();
		}

		return fs::join(curr_dir, libpath);
	}

	// libpath is relative, search it in
	//
	fs::pathlist::const_iterator it = searchpaths.begin();
	fs::pathlist::const_iterator it_end = searchpaths.end();

	while (it != it_end) {
		fs::path p = fs::join(*it, libpath);

		if (fs::exists(p))
			return p;
		++it;
	}

	return fs::path();
}


/**
 * @fn string dl::buildDlFileName (const string & basename)
 *
 * @brief Builds dynamic library file name according to platform.
 *
 * @param name Base name of dynamic library.
 *
 * @return Platform specific dynamic library file name.
 */


/**
 * @fn dl::handle dl::open (const string & path, bool global, bool resolve)
 *
 * @brief Loads the dynamic library file named by the @c path.
 *
 * @param path    Platform specific path to dynamic library file.
 * @param global  If @c true the symbols defined by this library
 *                will be made available for symbol resolution of
 *                subsequently loaded libraries, otherwise symbols
 *                defined in this library are not made available
 *                to resolve references in  subsequently loaded libraries.
 * @param resolve If @c true all undefined symbols in the library
 *                are resolved before open() returns, otherwise only resolve
 *                symbols as the code that references them is executed ((lazy binding)).
 *                If the symbol is never referenced, then it is never resolved.
 *                Lazy binding is only performed for function references; references
 *                to variables are always immediately bound when the library is loaded.
 *
 * @return Handler to the opened dynamic library file.
 */

//static const char * plugin_ctor_sym = "__plugin_ctor__";
//static const char * plugin_dtor_sym = "__plugin_dtor__";

/**
 * @brief Opens plug-in specified by path.
 *
 * @param name Unique name for plug-in.
 * @param path Platform specific path to the plug-in.
 *
 * @return Pointer to pluggable interface.
 */
//pfs::pluggable * dl::open_plugin (const string & name, const fs::path & path)
//{
//	bool global = false; // Avoid name conflicts
//	bool resolve = true;
//
//	if (_plugins.contains(name)) {
//        addError(string() << _u8("Duplicate plug-in with name: ") << name);
//        return nullptr;
//	}
//
//	dl::handle dlh = dl::open(path, global, resolve);
//
//	if (!dlh) {
//		addError(string() << _u8("Unable to load plug-in from ") << path);
//		return nullptr;
//	}
//
//	plugin_ctor_t ctor = reinterpret_cast<plugin_ctor_t>(dl::ptr(dlh, PFS_PLUGIN_CTOR_NAME));
//	if (!ctor) {
//		addError(string() << _u8("Constructor not found for plug-in: ")
//		        << name << " [" << path << ']');
//		return nullptr;
//	}
//
//	dl::_plugins.insert(name, dlh);
//
//	return ctor();
//}

/**
 * @brief Opens plug-in specified by name.
 *
 * @details Builds platform specific path for plug-in based on @c name, then attempts to find
 *          dynamic library file using @c dl::searchFile() method.
 *
 * @param name Plug-in name (used as base name for filename).
 * @param pluggable .
 * @return Pointer to pluggable interface if plug-in successfully opened, @c nullptr otherwise.
 *         In latter case error will be saved.
 */
//pfs::pluggable * dl::open_plugin (const string & name)
//{
//    string filename = buildDlFileName(name);
//    string path = searchFile(filename);
//
//    if (path.isEmpty()) {
//        addError(string() << _u8("Unable to find plug-in specified by name: ")
//                << name);
//        return nullptr;
//    }
//
//    return openPlugin(name, path);
//}

/**
 * @brief Closes the plug-in previously opened by @c dl::openPluggin() method.
 *
 * @param name Plug-in name.
 * @param pluggable Pointer to pluggable interface.
 * @return @c true if plug-in successfully closed, @c false otherwise.
 *         In latter case error will be saved.
 */
//bool dl::close_plugin (const string & name, pfs::pluggable * plugin)
//{
//    if (!plugin) {
//        addError(string() << _u8("Plaggable is null"));
//        return false;
//    }
//
//    plugin_map_type::iterator it = dl::_plugins.find(name);
//
//	if (it == dl::_plugins.end()) {
//		addError(string() << _u8("Plug-in not found, may be it was not load before: ")
//                << name);
//		return false;
//	}
//
//	dl::handle dlh = it.value();
//	PFS_ASSERT(dlh);
//
//	plugin_dtor_t dtor = reinterpret_cast<plugin_dtor_t>(dl::ptr(dlh, PFS_PLUGIN_DTOR_NAME));
//
//	if (!dtor) {
//		addError(string() << _u8("Destructor not found for plug-in: ") << name);
//		return false;
//	}
//
//	dl::_plugins.remove(name);
//
//	dtor(plugin);
//	return true;
//}

} // pfs

extern "C" int DLL_API dl_only_for_testing_purpose (void)
{
	int i = 0;
	++i;
	return i;
}
