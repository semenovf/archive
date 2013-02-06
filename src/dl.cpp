#include <cwt/dl.hpp>

/**
 * @fn String Dl::buildDlFileName (const String &basename)
 *
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */


extern "C" int DLL_API dl_only_for_testing_purpose(void)
{
	int i = 0;
	i++;
	return i;
}
