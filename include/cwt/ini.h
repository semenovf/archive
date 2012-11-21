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

typedef struct _CwtIniContext {
	size_t        max_tokens; /* maximum number of tokens in INI file instruction line */
	UINT          flags;
	CwtChannel   *pchan;
	size_t        line;
	CwtHashTable *directives;
	BOOL        (*defaultCallback)(struct _CwtIniContext*, const CWT_CHAR **argv, size_t argc);
	void        (*onError)(struct _CwtIniContext*, const CWT_CHAR*);
} CwtIniContext;

typedef BOOL (*CwtIniCallback)(struct _CwtIniContext*, const CWT_CHAR **argv, size_t argc);

typedef struct CwtIniNS {
	struct _CwtIniContext* (*create)  (void);
	struct _CwtIniContext* (*createWithFlags) (UINT flags, size_t max_tokens);
	void          (*free)    (struct _CwtIniContext*);
	BOOL          (*parse)   (struct _CwtIniContext*, CwtChannel*);
	void          (*error)   (struct _CwtIniContext*, const CWT_CHAR *errstr);
	void          (*addDirective) (struct _CwtIniContext*, const CWT_CHAR *directive, CwtIniCallback handler);
	void          (*setDefaultDirective) (struct _CwtIniContext*, CwtIniCallback handler);
	size_t        (*line)    (struct _CwtIniContext*);
} CwtIniNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtIniNS* cwtIniNS(void);
EXTERN_C_END

#endif /* __CWT_INI_H__ */
