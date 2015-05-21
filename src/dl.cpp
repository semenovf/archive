#include "pfs/dl.hpp"
#include "pfs/fs.hpp"

namespace pfs {

stringlist dl::_globalSearchPath;

/**
 * @note  internal
 *
 * @brief Searches file in directories added by addSearchPath()
 *
 * @param filename
 * @param realPath
 * @return
 */
string dl::searchFile (const string & filename)
{
	pfs::fs fs;

	if (fs.exists(filename)) {
		return string(filename);
	}

	if (!fs.isAbsolute(filename)) {
		stringlist::const_iterator it = _searchPath.cbegin();
		stringlist::const_iterator itEnd = _searchPath.cend();

		while (it != itEnd) {
						string r(*it);
			r += fs.separator();
			r += filename;
			if (fs.exists(r))
				return r;
			++it;
		}

		it = _globalSearchPath.cbegin();
		itEnd = _globalSearchPath.cend();

		while (it != itEnd) {
			string r(*it);
			r += fs.separator();
			r += filename;
			if (fs.exists(r))
				return r;
			++it;
		}

	}

	return string();
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


typedef bool (* __plugin_ctor) (void *);
typedef bool (* __plugin_dtor) (void *);
static const char * __plugin_ctor_sym = "__plugin_ctor__";
static const char * __plugin_dtor_sym = "__plugin_dtor__";

/**
 * @brief Opens plug-in specified by path.
 *
 * @param name Unique name for plug-in.
 * @param path Platform specific path to the plug-in.
 * @param pluggable Pointer to pluggable interface.
  */
bool dl::openPlugin (const string & name, const string & path, pfs::pluggable * pluggable)
{
    if (!pluggable) {
        addError(string() << _Tr("Plaggable is null"));
        return false;
    }

	bool global = false; // Avoid name conflicts
	bool resolve = true;
	dl::handle dlh = dl::open(path, global, resolve);

	if (_plugins.contains(name)) {
        addError(string() << _Tr("Duplicate plug-in with name: ") << name);
        return false;
	}

	if (!dlh) {
		addError(string() << _Tr("Unable to load plug-in from ") << path);
		return false;
	}

	__plugin_ctor ctor = reinterpret_cast<__plugin_ctor>(dl::ptr(dlh, __plugin_ctor_sym));
	if (!ctor) {
		addError(string() << _Tr("Constructor not found for plug-in: ")
		        << name << " [" << path << ']');
		return false;
	}

	dl::_plugins.insert(name, dlh);

	return ctor(pluggable);
}

/**
 * @brief Opens plug-in specified by name.
 *
 * @details Builds platform specific path for plug-in based on @c name, then attempts to find
 *          dynamic library file using @c dl::searchFile() method.
 *
 * @param name Plug-in name (used as base name for filename).
 * @param pluggable Pointer to pluggable interface.
 * @return @c true if plug-in successfully opened, @c false otherwise.
 *         In latter case error will be saved.
 */
bool dl::openPlugin (const string & name, pfs::pluggable * pluggable)
{
    if (!pluggable) {
        addError(string() << _Tr("Plaggable is null"));
        return false;
    }

    string filename = buildDlFileName(name);
    string path = searchFile(filename);

    if (path.isEmpty()) {
        addError(string() << _Tr("Unable to find plug-in specified by name: ")
                << name);
        return false;
    }

    return openPlugin(name, path, pluggable);
}

/**
 * @brief Closes the plug-in previously opened by @c dl::openPluggin() method.
 *
 * @param name Plug-in name.
 * @param pluggable Pointer to pluggable interface.
 * @return @c true if plug-in successfully closed, @c false otherwise.
 *         In latter case error will be saved.
 */
bool dl::closePlugin (const string & name, pfs::pluggable * pluggable)
{
    if (!pluggable) {
        addError(string() << _Tr("Plaggable is null"));
        return false;
    }

	pfs::map<string, dl::handle>::iterator it = dl::_plugins.find(name);

	if (it == dl::_plugins.end()) {
		addError(string() << _Tr("Plug-in not found, may be it was not load before: ")
                << name);
		return false;
	}

	dl::handle dlh = it.value();
	PFS_ASSERT(dlh);

	__plugin_dtor dtor = reinterpret_cast<__plugin_dtor>(dl::ptr(dlh, __plugin_dtor_sym));

	if (!dtor) {
		addError(string() << _Tr("Destructor not found for plug-in: ") << name);
		return false;
	}

	dl::_plugins.remove(name);

	return dtor(pluggable);
}

} // pfs

extern "C" int DLL_API dl_only_for_testing_purpose (void)
{
	int i = 0;
	++i;
	return i;
}
