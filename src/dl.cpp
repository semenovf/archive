#include "../include/cwt/dl.hpp"
#include "../include/cwt/safeformat.hpp"
#include "../include/cwt/logger.hpp"

CWT_NS_BEGIN

Vector<String> Dl::searchPath;
Map<String, Dl::Handle> Dl::plugins;

/**
 * @fn String Dl::buildDlFileName (const String &basename)
 *
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */


/**
 * @fn Dl::Handle Dl::open (const String & path, bool global, bool resolve)
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


typedef bool (* __plugin_ctor) (const void *);
typedef bool (* __plugin_dtor) (const void *);
static const char * __plugin_ctor_sym = "__cwt_plugin_ctor__";
static const char * __plugin_dtor_sym = "__cwt_plugin_dtor__";

bool Dl::pluginOpen(const String & name, const String & path, void * pluggable)
{
	Dl::Handle dlh = Dl::open(path);

	if (!dlh) {
		Logger::error(_Fr("Unable to load plugin from %s") % path);
		return false;
	}

	__plugin_ctor ctor = reinterpret_cast<__plugin_ctor>(Dl::symbol(dlh, __plugin_ctor_sym));
	if (!ctor) {
		Logger::error(_Fr("Constructor (%s) not found at %s") % __plugin_ctor_sym % path);
		return false;
	}

	Dl::plugins.insert(name, dlh);

	return ctor(pluggable);
}

bool Dl::pluginClose(const String & name, void * pluggable)
{
	Map<String, Handle>::iterator it = Dl::plugins.find(name);

	if (it == Dl::plugins.end()) {
		Logger::error(_Fr("Plugin '%s' not found, may be it was not load before") % name);
		return false;
	}

	Dl::Handle dlh = it->second;
	CWT_ASSERT(dlh);

	__plugin_dtor dtor = reinterpret_cast<__plugin_dtor>(Dl::symbol(dlh, __plugin_dtor_sym));
	if (!dtor) {
		Logger::error(_Fr("Destructor (%s) not found for plugin %s") % __plugin_dtor_sym % name);
		return false;
	}

	Dl::plugins.remove(name);

	return dtor(pluggable);
}



extern "C" int DLL_API dl_only_for_testing_purpose(void)
{
	int i = 0;
	i++;
	return i;
}

CWT_NS_END
