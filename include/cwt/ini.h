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
#include <cwt/hashtab.h>


typedef enum _CwtIniFlag {
	  Cwt_IniFlag_DirectiveIgnoreCase    = 0x01
	, Cwt_IniFlag_DirectiveIgnoreUnknown = 0x02
} CwtIniFlag;

typedef struct _CwtIniHandler {
	size_t        max_tokens; /* maximum number of tokens in INI file instruction line */
	UINT          flags;
	CwtChannel   *pchan;
	size_t        line;
	CwtHashTable *directives;
	BOOL        (*defaultCallback)(struct _CwtIniHandler*, const CWT_CHAR **argv, size_t argc);
	void        (*on_error)(struct _CwtIniHandler*, const CWT_CHAR*);
} CwtIniHandler;

typedef BOOL (*CwtIniCallback)(struct _CwtIniHandler*, const CWT_CHAR **argv, size_t argc);

typedef struct CwtIniNS {
	struct _CwtIniHandler* (*create)  (void);
	struct _CwtIniHandler* (*createWithFlags) (UINT flags, size_t max_tokens);
	void          (*free)    (struct _CwtIniHandler*);
	BOOL          (*parse)   (struct _CwtIniHandler*, CwtChannel*);
	void          (*error)   (struct _CwtIniHandler*, const CWT_CHAR *errstr);
	void          (*onError) (struct _CwtIniHandler*, void (*callback)(struct _CwtIniHandler*, const CWT_CHAR*));
	void          (*addDirective) (struct _CwtIniHandler*, const CWT_CHAR *directive, CwtIniCallback handler);
	void          (*setDefaultDirective) (struct _CwtIniHandler*, CwtIniCallback handler);
	size_t        (*line)    (struct _CwtIniHandler*);
} CwtIniNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtIniNS* cwtIniNS(void);
EXTERN_C_END

#endif /* __CWT_INI_H__ */
