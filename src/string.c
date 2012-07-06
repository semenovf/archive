/*
 * string.c
 *
 *  Created on: 18.06.2012
 *      Author: user
 */


#include <cwt/string.h>


const CHAR* CWT_CONST_EMPTYSTR = "";
const CHAR* CWT_CONST_NULLSTR = "<null>";


CHAR* cwtStrNdup(const CHAR *s, size_t n)
{
	CHAR *s0 = NULL;

	s0 = CWT_MALLOCA(CHAR, n+1);
	cwtStrNcpy(s0, s, n);
	s0[n] = '\x0';

	return s0;
}
