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


typedef enum CwtIniFlag {
	Cwt_IniFlag_DirectiveIgnoreCase = 0x01
};

typedef void* CwtIniHandler;
typedef BOOL (*CwtIniCallback)(CwtIniHandler, const CWT_CHAR* argv[], int argc);

typedef struct CwtIniNS {
	CwtIniHandler (*create)  (void);
	CwtIniHandler (*createWithFlags) (UINT flags, size_t max_tokens);
	void          (*free)    (CwtIniHandler);
	BOOL          (*parse)   (CwtIniHandler, CwtChannel*);
	void          (*error)   (CwtIniHandler, const CWT_CHAR *errstr);
	void          (*onError) (CwtIniHandler, void (*callback)(CwtIniHandler, const CWT_CHAR*));
	void          (*addDirective) (CwtIniHandler, const CWT_CHAR *directive, CwtIniCallback handler);
	void          (*setDefaultDirective) (CwtIniHandler, CwtIniCallback handler);
	size_t        (*line)    (CwtIniHandler);
} CwtIniNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtIniNS* cwtIniNS(void);
EXTERN_C_END

#endif /* __CWT_INI_H__ */
