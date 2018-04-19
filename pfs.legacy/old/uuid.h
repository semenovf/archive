/*
** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
** Digital Equipment Corporation, Maynard, Mass.
** Copyright (c) 1998 Microsoft.
** To anyone who acknowledges that this file is provided "AS IS"
** without any express or implied warranty: permission to use, copy,
** modify, and distribute this file for any purpose is hereby
** granted without fee, provided that the above copyright notices and
** this notice appears in all source code copies, and that none of
** the names of Open Software Foundation, Inc., Hewlett-Packard
** Company, Microsoft, or Digital Equipment Corporation be used in
** advertising or publicity pertaining to distribution of the software
** without specific, written prior permission. Neither Open Software
** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
** Equipment Corporation makes any representations about the
** suitability of this software for any purpose.
*/

/**
 * @file   uuid.cpp
 * @author see Copyright above
 * @date   Feb 20, 2013 1:56:25 PM
 *
 * @brief
 * @see http://www.ietf.org/rfc/rfc4122.txt (A Universally Unique IDentifier (UUID) URN Namespace)
 */

#ifndef __UUID_H__
#define __UUID_H__

#include <cwt/cwt.h>

EXTERN_C_BEGIN
DLL_API int uuid_create(uuid_t * uuid);
DLL_API void uuid_create_md5_from_name(uuid_t *uuid, uuid_t nsid, const void *name, int namelen);
DLL_API void uuid_create_sha1_from_name(uuid_t *uuid, uuid_t nsid, const void *name, int namelen);
DLL_API int uuid_compare(uuid_t *u1, uuid_t *u2);
EXTERN_C_END

#endif /* __UUID_H__ */
