#include <cwt/dl.h>
#include <cwt/strbuf.h>
#include <cwt/logger.h>

static DlHandle __open(const CWT_CHAR *path, BOOL global, BOOL resolve);
static DlSymbol __symbol(DlHandle h, const CWT_CHAR *name);
static void     __close(DlHandle h);
static BOOL     __buildDlFileName(const CWT_CHAR *name, CWT_CHAR libname[], int *psz);

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


static DlSymbol __symbol(DlHandle h, const CWT_CHAR *sym_name)
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
 * @param psz size of buffer @c libname
 * @return @c TRUE if building is successfull, or @c FALSE if buffer too small
 */
static BOOL __buildDlFileName(const CWT_CHAR *name, CWT_CHAR libname[], int *psz)
{
	CwtStringBufferNS *sbns = cwtStringBufferNS();
	CwtStringBuffer sb;

	CWT_ASSERT(psz);

	sbns->init(&sb);

#ifdef CWT_OS_WIN
	sbns->append(&sb, name);
	sbns->append(&sb, _T(".dll"));
#elif defined(CWT_OS_LINUX)
	sbns->append(&sb, _T(".lib"));
	sbns->append(&sb, name);
	sbns->append(&sb, _T(".so"));
#else
#	error OS not supported
#endif

	if( sbns->size(&sb) + 1 >  *psz ) {
		*psz = sbns->size(&sb) + 1;
		sbns->destroy(&sb);
		return FALSE;
	}

	cwtStrNcpy(libname, sbns->cstr(&sb), sbns->size(&sb));
	*psz = sbns->size(&sb);
	sbns->destroy(&sb);
	return TRUE;
}



