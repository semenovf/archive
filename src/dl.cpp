#include "../include/cwt/dl.hpp"
#include "../include/cwt/fs.hpp"

namespace cwt {

pfs::vector<pfs::string> dl::globalSearchPath;

/**
 * @brief Searches file in directories added by addSearchPath()
 *
 * @param filename
 * @param realPath
 * @return
 */
pfs::string dl::searchFile (const pfs::string & filename)
{
	cwt::fs fs;

	if (fs.exists(filename)) {
		return pfs::string(filename);
	}

	if (!fs.isAbsolute(filename)) {
		pfs::vector<pfs::string>::const_iterator it = searchPath.cbegin();
		pfs::vector<pfs::string>::const_iterator itEnd = searchPath.cend();
		while (it != itEnd) {
			pfs::string r = *it + fs.separator() + filename;
			if (fs.exists(r))
				return r;
			++it;
		}

		it = globalSearchPath.cbegin();
		itEnd = globalSearchPath.cend();
		while (it != itEnd) {
			pfs::string r = *it + fs.separator() + filename;
			if (fs.exists(r))
				return r;
			++it;
		}

	}

	return pfs::string();
}


/**
 * @fn pfs::string Dl::buildDlFileName (const pfs::string &basename)
 *
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */


/**
 * @fn Dl::Handle Dl::open (const pfs::string & path, bool global, bool resolve)
 *
 * @brief Loads the dynamic library file named by the @c path.
 *
 * @param path    Path to file with dynamic library.
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
 * @return
 */


typedef bool (* __plugin_ctor) (void *);
typedef bool (* __plugin_dtor) (void *);
static const char * __plugin_ctor_sym = "__cwt_plugin_ctor__";
static const char * __plugin_dtor_sym = "__cwt_plugin_dtor__";

bool dl::pluginOpen(const pfs::string & name, const pfs::string & path, void * pluggable)
{
	dl::handle dlh = dl::open(path);

	if (!dlh) {
		pfs::string errstr;
		errstr << _Tr("Unable to load plugin from ")
				<< path;
		addError(errstr);
		return false;
	}

	__plugin_ctor ctor = reinterpret_cast<__plugin_ctor>(dl::ptr(dlh, __plugin_ctor_sym));
	if (!ctor) {
		pfs::string errstr;
		errstr << path
				<< ": "
				<< _Tr("Constructor not found");
		addError(errstr);
		return false;
	}

	dl::plugins.insert(name, dlh);

	return ctor(pluggable);
}

bool dl::pluginClose(const pfs::string & name, void * pluggable)
{
	pfs::map<pfs::string, dl::handle>::iterator it = dl::plugins.find(name);

	if (it == dl::plugins.end()) {
		pfs::string errstr;
		errstr << name
				<< ": "
				<< _Tr("Plugin not found, may be it was not load before");
		addError(errstr);
		return false;
	}

	dl::handle dlh = it->second;
	PFS_ASSERT(dlh);

	__plugin_dtor dtor = reinterpret_cast<__plugin_dtor>(dl::ptr(dlh, __plugin_dtor_sym));

	if (!dtor) {
		pfs::string errstr;
		errstr << name
				<< ": "
				<< _Tr("Destructor not found");
		addError(errstr);
		return false;
	}

	dl::plugins.remove(name);

	return dtor(pluggable);
}

} // cwt

extern "C" int DLL_API dl_only_for_testing_purpose (void)
{
	int i = 0;
	i++;
	return i;
}
