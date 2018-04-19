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
#include <cwt/strlist.h>

#define CWT_END_OPTIONS { NULL, 0, Cwt_Opt_BOOL, NULL, NULL, NULL }

typedef enum _CwtArgvType {
	  Cwt_Argv_ShortOpt
	, Cwt_Argv_LongOpt
	, Cwt_Argv_LongOptWithArg
	, Cwt_Argv_SingleDash
	, Cwt_Argv_DoubleDash
	, Cwt_Argv_Arg
} CwtArgvType;

typedef enum _CwtOptionType {
	  Cwt_Opt_BOOL
	, Cwt_Opt_INT
	, Cwt_Opt_REAL
	, Cwt_Opt_TEXT
} CwtOptionType;

typedef enum _CwtOptionIteratorType {
	  Cwt_OptionIterator_Default
	/*, CwtOptIt_Alternative*/
} CwtOptionIteratorType;

typedef struct _CwtOption {
	CWT_CHAR     *longname;  /* long option or NULL */
	CWT_CHAR      shortname; /* short option or 0 */
	CwtOptionType optType;   /* option's argument type (see CwtOptType) */
	void         *arg;       /* pointer to opttion's argument value or NULL if option has no argument */
	BOOL        (*validator)(const void*);  /* validation function for option argument */
	CWT_CHAR     *desc;      /* option's description (for automatically generated usage) */
} CwtOption;

typedef struct _CwtOptionIterator {
	void        (*begin)        (struct _CwtOptionIterator *it, int argc, char **argv);
	BOOL        (*hasMore)      (struct _CwtOptionIterator *it);
	CwtArgvType (*next)         (struct _CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg);
	void        (*free)         (struct _CwtOptionIterator *it);
	void        (*onError)      (const CWT_CHAR *errstr);
	void        (*onOption)     (CwtOption *popt);
	void        (*onSingleDash) (void);
	void        (*onDoubleDash) (void);
	void        (*onArgument)   (const CWT_CHAR *s);
} CwtOptionIterator;

typedef struct _CwtOptionsNS {
	CwtOptionIterator* (*createIterator)(CwtOptionIteratorType itType);
	void (*printUsage) (const CWT_CHAR *progname
			, const CWT_CHAR *copyright
			, const CWT_CHAR *usageSummary
			, const CwtOption *optset
			, FILE *out);
	BOOL (*parse)      (int argc, char **argv, CwtOption *options, CwtStrList *args, CwtOptionIteratorType itType);
	BOOL (*parseWithIterator) (CwtOption *options, CwtStrList *args, CwtOptionIterator *it);
} CwtOptionsNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtOptionsNS* cwt_options_ns(void);
EXTERN_C_END


#endif /* __CWT_OPTIONS_H__ */
