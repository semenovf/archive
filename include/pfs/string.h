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

DLL_API char * pfs_long_to_string (long_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_ulong_to_string (ulong_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_real_to_string (real_t n, char f, int prec, char * buf, int bufsz);

EXTERN_C_END

#endif /* __PFS_STRING_H__ */
