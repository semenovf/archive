/*
 * ini.h
 *
 *  Created on: 06.12.2011
 *      Author: user
 */

#ifndef __CWT_INI_H__
#define __CWT_INI_H__

#include <cwt/types.h>
#include <cwt/io/channel.h>


/* maximum number of tokens in INI file instruction line */
#ifndef CWT_INI_MAX_TOKENS
#	define CWT_INI_MAX_TOKENS 128
#endif

typedef void* CwtIniHandler;
typedef BOOL (*CwtIniCallback)(CwtIniHandler, const CWT_CHAR* argv[], int argc);

typedef struct CwtIniNS {
	CwtIniHandler (*create)  (void);
	void          (*free)    (CwtIniHandler);
	BOOL          (*parse)   (CwtIniHandler, CwtChannel*);
	void          (*error)   (CwtIniHandler, const CWT_CHAR *errstr);
	void          (*onError) (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
	void          (*addDirective) (CwtIniHandler, const CWT_CHAR *directive, CwtIniCallback handler);
	size_t        (*line)    (CwtIniHandler);
} CwtIniNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtIniNS* cwtIniNS(void);
EXTERN_C_END

#endif /* __CWT_INI_H__ */
