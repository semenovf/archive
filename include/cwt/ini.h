/*
 * ini.h
 *
 *  Created on: 06.12.2011
 *      Author: user
 */

#ifndef __CWT_INI_H__
#define __CWT_INI_H__

#include <cwt/types.h>

typedef enum CWT_INI_STD_HANDLER
{
	  INI_HANDLER_NULL
	, INI_HANDLER_DEBUG
} CWT_INI_STD_HANDLER;

typedef struct CwtIniHandlerBase {
	const CWT_CHAR* file;
	size_t line;
	BOOL error;
	void (*on_start_document)(struct CwtIniHandlerBase* h);
	void (*on_end_document)(struct CwtIniHandlerBase* h);
	void (*on_start_line)(struct CwtIniHandlerBase* h);
	void (*on_end_line)(struct CwtIniHandlerBase* h);
	void (*on_token)(struct CwtIniHandlerBase* h, const CWT_CHAR* token);
	void (*on_error)(struct CwtIniHandlerBase* h, const CWT_CHAR* errstr);
} CwtIniHandlerBase;


#define CWT_INI_DECL_ARGS CwtIniHandlerBase* h, int argc, const CWT_CHAR* argv[]

typedef struct CwtIniRule {
	const CWT_CHAR *cmd;
	int min_argc;
	void (*callback)(CWT_INI_DECL_ARGS);
} CwtIniRule;

/* maximum number of tokens in INI file instruction line */
#ifndef CWT_INI_MAX_TOKENS
#	define CWT_INI_MAX_TOKENS 64
#endif

EXTERN_C_BEGIN

DLL_API_EXPORT BOOL cwtLoadIni(const CWT_CHAR* path, CwtIniHandlerBase* handler);
DLL_API_EXPORT BOOL cwtLoadIniStd(const CWT_CHAR* path, CWT_INI_STD_HANDLER handler);
DLL_API_EXPORT BOOL cwtLoadIniByRules(const CWT_CHAR* path, CwtIniRule *rules, void (*on_error)(CwtIniHandlerBase*, const char*));

EXTERN_C_END

#endif /* __CWT_INI_H__ */
