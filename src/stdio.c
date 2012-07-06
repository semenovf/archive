/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: wladt
 */


#include <cwt/stdio.h>
#include <stdio.h>

static CwtStdioNS __cwtStdioNS = {
#ifdef CWT_UNICODE
	vswprintf
#else
	vsprintf
#endif

};


DLL_API_EXPORT CwtStdioNS* cwtStdioNS(void)
{
	return &__cwtStdioNS;
}


