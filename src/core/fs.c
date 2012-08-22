/*
 * fs.c
 *
 *  Created on: 22.08.2012
 *      Author: user
 */

#include <stdarg.h>
#include <cwt/fs.h>
#include <cwt/str.h>

const CWT_CHAR __fs_separator(void);
CWT_CHAR*      __fs_buildPath(const CWT_CHAR *arg0, ...);
CWT_CHAR*      __fs_buildPathArgv(const CWT_CHAR *argv[], size_t n);

static CwtFileSystemNS __cwtFileSystemNS = {
	  __fs_separator
	, __fs_buildPath
	, __fs_buildPathArgv
};

static CwtStrNS *__strNS = NULL;
static CwtStringNS *__stringNS = NULL;

#if defined(CWT_OS_WIN) || defined(CWT_OS_DOS)
	static CWT_CHAR __fs_separator_char = _T('\\');
#elif defined(CWT_OS_UNIX)
	static CWT_CHAR __fs_separator_char = _T('/');
#else
	static CWT_CHAR __fs_separator_char = _T('/');
#endif




DLL_API_EXPORT CwtFileSystemNS* cwtFileSystemNS(void)
{
	if( !__strNS ) {
		__strNS    = cwtStrNS();
		__stringNS = cwtStringNS();
	}
	return &__cwtFileSystemNS;
}


const CWT_CHAR __fs_separator(void)
{
	return __fs_separator_char;
}

CWT_CHAR* __fs_buildPath(const CWT_CHAR *arg0, ...)
{
	CwtString *sbuf;
	CWT_CHAR *s = NULL;
	const CWT_CHAR *arg;
	va_list args;

	va_start(args, arg0);

	sbuf = __stringNS->create();
	__stringNS->append(sbuf, arg0);

	while( (arg = va_arg(args,const CWT_CHAR*)) ) {
		__stringNS->appendChar(sbuf, __fs_separator_char);
		__stringNS->append(sbuf, arg);
	}

	va_end(args);

	s = __strNS->strdup(__stringNS->cstr(sbuf));
	__stringNS->free(sbuf);

	return s;
}

CWT_CHAR* __fs_buildPathArgv(const CWT_CHAR *argv[], size_t n)
{
	CWT_CHAR *s = NULL;

	if( n > 0 ) {
		size_t i;
		CwtString *sbuf;

		sbuf = __stringNS->create();

		__stringNS->append(sbuf, argv[0]);

		for( i = 1; i < n; i++ ) {
			__stringNS->appendChar(sbuf, __fs_separator_char);
			__stringNS->append(sbuf, argv[i]);
		}

		s = __strNS->strdup(__stringNS->cstr(sbuf));
		__stringNS->free(sbuf);
	}

	return s;
}
