/*
 * string.h
 *
 *  Created on: 10 янв. 2015 г.
 *      Author: wladt
 */

#ifndef __PFS_STRING_H__
#define __PFS_STRING_H__

#include <pfs.h>

EXTERN_C_BEGIN

// XXX Moved to pfs/cast/string.hpp
DLL_API char * pfs_integral_to_string  (integral_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_uintegral_to_string (uintegral_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_real_to_string  (real_t n, char f, int prec, char * buf, int * bufsz);
DLL_API int    pfs_latin1_to_digit (char latin1);

EXTERN_C_END

#endif /* __PFS_STRING_H__ */
