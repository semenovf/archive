/*
 * abnf.h
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */

#ifndef __CWT_ABNF_H__
#define __CWT_ABNF_H__

#include <cwt/types.h>

typedef struct _CwtAbnf {
} CwtAbnf;

typedef struct _CwtAbnfNS {
	ssize_t   (*parse)     (const CWT_CHAR *abnf_string);
	ssize_t   (*parseFile) (const CWT_CHAR *filepath);
	void      (*setNewLine)(CwtNewLine nl);
} CwtAbnfNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtAbnfNS* cwtAbnfNS(void);
EXTERN_C_END

#endif /* __CWT_ABNF_H__ */
