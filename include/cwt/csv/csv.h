/*
 * csv.h
 *
 *  Created on: 27.07.2012
 *      Author: user
 */

#ifndef __CWT_CSV_H__
#define __CWT_CSV_H__

#include <cwt/types.h>
#include <cwt/io/channel.h>
#include <cwt/dbi/dbi.h>
#include <cwt/dbi/ddi.h>


typedef struct _CwtCsvContext {
	CWT_CHAR      *separator;
} CwtCsvContext;

typedef BOOL (*CwtCsvCallback)(CwtCsvContext*, const CWT_CHAR **argv, size_t argc);

typedef struct CwtCsvNS {
	CwtCsvContext*  (*create)  (void);
	CwtCsvContext*  (*createWithArgs) (const CWT_CHAR *separator, size_t max_tokens);
	void            (*free)       (CwtCsvContext*);
	void            (*write)      (CwtCsvContext*, CwtChannel*, const CWT_CHAR *argv[], size_t argc);
	BOOL            (*parse)      (CwtCsvContext*, CwtChannel*);
	void            (*setOnRow)   (CwtCsvContext*, CwtCsvCallback cb);
	void            (*error)      (CwtCsvContext*, const CWT_CHAR *format);
	void            (*setOnError) (CwtCsvContext*, void (*ecallback)(CwtCsvContext*, const CWT_CHAR*));
	size_t          (*line)       (CwtCsvContext*);

	/* Simple API for CSV (SAC) */
	void            (*begin)      (CwtCsvContext*, CwtChannel*);
	size_t          (*header)     (CwtCsvContext*);
	void            (*titles)     (CwtCsvContext*, CWT_CHAR **argv, size_t argc);
	BOOL            (*next)       (CwtCsvContext*);
	size_t          (*columnsCount) (CwtCsvContext*);
	size_t          (*row)        (CwtCsvContext*, const CWT_CHAR **argv, size_t argc);
	const CWT_CHAR* (*column)     (CwtCsvContext*, const CWT_CHAR *name);

	BOOL            (*persist)    (CwtCsvContext*, CwtDBHandler *dbh, CwtDDITable *table);

	BOOL            (*setValidator) (CwtCsvContext*, const CWT_CHAR *name, BOOL (*)(const CWT_CHAR*));
} CwtCsvNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtCsvNS* cwtCsvNS(void);
EXTERN_C_END

#endif /* __CWT_CSV_H__ */
