/*
 * options.h
 *
 *  Created on: Sep 17, 2012
 *      Author: wladt
 */

#ifndef __CWT_OPTIONS_H__
#define __CWT_OPTIONS_H__

#include <cwt/types.h>
#include <cwt/stdio.h>

#define CWT_END_OPTIONS { _T(""), 0, CwtOpt_BOOL, NULL, NULL, _T("") }

typedef enum _CwtArgvType {
	  CwtArgv_ShortOpt
	, CwtArgv_LongOpt
	, CwtArgv_LongOptWithArg
	, CwtArgv_SingleDash
	, CwtArgv_DoubleDash
	, CwtArgv_Arg
} CwtArgvType;

typedef enum _CwtOptionType {
	  CwtOpt_BOOL
	, CwtOpt_INT
	, CwtOpt_REAL
	, CwtOpt_TEXT
} CwtOptionType;


typedef struct _CwtOption {
	CWT_CHAR     *longname;  /* "длинная" опция или NULL */
	CWT_CHAR      shortname; /* "короткая" опция или 0 */
	CwtOptionType optType;   /* тип аргумента (см. CwtOptType) */
	void         *arg;       /* указатель на значение аргумента опции или NULL, если аргумент не используется */
	CWT_CHAR     *desc;      /* описание опции (используется при автоматической генерации справки) */
	BOOL        (*validator)(const void*);  /* функция-валидатор аргумента опции */
} CwtOption;

typedef struct _CwtOptionIterator {
	char **argv;
	int    argc;
	int    argi;
} CwtOptionIterator;


typedef struct _CwtOptionsHandler
{
	void (*option)     (CwtOption *opt);
	void (*doubleDash) (void);
	void (*singleDash) (void);
	void (*argument)   (const CWT_CHAR *arg);
	void (*on_error)   (const CWT_CHAR *errstr);
} CwtOptionsHandler;

typedef struct _CwtOptionsNS {
	void (*printUsage)(const CWT_CHAR *progname, const CwtOption *optset, FILE *out);
	BOOL (*parse)(int argc, char **argv, CwtOption *options, CwtOptionsHandler *handler);
	void (*begin)(CwtOptionIterator *it, int argc, char **argv);
	BOOL (*hasMore)(CwtOptionIterator *it);
	CwtArgvType (*next)(CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg);
} CwtOptionsNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtOptionsNS* cwtOptionsNS(void);
/*
DLL_API_EXPORT CwtOptionsHandler* cwtDebugOptionsHandler(void);
DLL_API_EXPORT CwtOptionsHandler* cwtDefaultOptionsHandler(void);
DLL_API_EXPORT CwtOptionsHandler* cwtShortOptionsOnlyHandler(void);
DLL_API_EXPORT CwtOptionsHandler* cwtLongOptionsOnlyHandler(void);
*/
EXTERN_C_END


#endif /* __CWT_OPTIONS_H__ */
