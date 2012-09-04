/*
 * poll.c
 *
 *  Created on: Sep 4, 2012
 *      Author: wladt
 */

#include <cwt/sys.h>


static CwtSysNS __cwtSysNS = {
	0
};


DLL_API_EXPORT CwtSysNS* cwtSysNS(void)
{
	return &__cwtSysNS;
}
