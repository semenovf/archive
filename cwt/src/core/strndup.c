/**
 * @file   strndup.c
 * @author wladt
 * @date   Nov 29, 2012 4:45:06 PM
 *
 * @brief
 */

/* Implement the strndup function.
   Copyright (C) 2005 Free Software Foundation, Inc.
   Written by Kaveh R. Ghazi <ghazi@caip.rutgers.edu>.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "cwt/str.h"
#include <string.h>
#include <stdlib.h>

DLL_API_EXPORT char* cwt_strndup (const char *s, size_t n)
{
	char *result;
	size_t len = strlen (s);

	if (n < len)
		len = n;

	result = (char *) malloc (len + 1);
	if (!result)
		return 0;

	result[len] = '\0';
	return (char *) memcpy (result, s, len);
}
