#include <cwt/dl.h>
#include <cwt/string.h>
#include <cwt/logger.h>

static DlHandle __open(const CWT_CHAR *path, BOOL global, BOOL resolve);
static DlSymbol __symbol(DlHandle h, const char *name);
static void     __close(DlHandle h);
static void     __buildDlFileName(const CWT_CHAR *name, CwtString *libname);

static CwtDlNS __cwtDlNS = {
	  __open
	, __symbol
	, __close
	, __buildDlFileName
};


DLL_API_EXPORT CwtDlNS* cwtDlNS(void)
{
	return &__cwtDlNS;
}

DlHandle __open( const CWT_CHAR *path, BOOL global, BOOL resolve )
{
	DlHandle h = NULL;

#ifdef CWT_CC_MSC
	DWORD dwFlags = 0;
	CWT_UNUSED(global);

	if( !resolve ) dwFlags |= DONT_RESOLVE_DLL_REFERENCES;

	h = LoadLibraryEx(path, NULL, dwFlags);

	if( !h ) {
		printf_error( _Tr("%s: failed to open dynamic library: error code=%lu"), path, GetLastError() );
	} else {
	   // use the result in a call to GetProcAddress
	}
#else
	dlerror(); // clear error
	h = dlopen( path, (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ) );
	if( !m_handle ) {
		printf_error( _Tr("%s: failed to open dynamic library: %s"), path, dlerror() );
	}
#endif

	return h;
}


static DlSymbol __symbol(DlHandle h, const char *sym_name)
{
	DlSymbol sym = NULL;

	CWT_ASSERT(sym_name);

#ifdef CWT_CC_MSC
	sym = GetProcAddress(h, sym_name);
	if( !sym ) {
		printf_error(_Tr("%s: symbol not found: error code=%lu"), sym_name, GetLastError());
	}
#else
	dlerror(); /*clear error*/
	DlSymbol sym = dlsym( h, sym_name );
	if( !sym ) {
		printf_error(_Tr("%s: symbol not found: error code=%lu"), sym_name, dlerror());
	}
#endif
	return sym;
}


void __close(DlHandle h)
{
	if( h != (DlHandle)0) {
#ifdef CWT_CC_MSC
		FreeLibrary(h);
#else
		dlerror(); /*clear error*/
		dlclose(h);
#endif
		h = (DlHandle)0;
	}
}


/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
static void __buildDlFileName(const CWT_CHAR *name, CwtString *libname)
{
	CwtStringNS *srtns = cwtStringNS();

#ifdef CWT_OS_WIN
	srtns->append(libname, name);
	srtns->append(libname, _T(".dll"));
#elif defined(CWT_OS_LINUX)
	srtns->append(libname, _T(".lib"));
	srtns->append(libname, name);
	srtns->append(libname, _T(".so"));
#else
#	error OS not supported
#endif
}



