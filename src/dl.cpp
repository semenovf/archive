#include <cwt/dl.hpp>

CWT_NS_BEGIN

Vector<String> Dl::searchPath;

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


extern "C" int DLL_API dl_only_for_testing_purpose(void)
{
	int i = 0;
	i++;
	return i;
}

CWT_NS_END
