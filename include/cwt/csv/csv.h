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

typedef void* CwtCsvHandler;
typedef BOOL (*CwtCsvCallback)(CwtCsvHandler, const CWT_CHAR* argv[], size_t argc);

typedef struct CwtCsvNS {
	CwtCsvHandler (*create)  (void);
	CwtCsvHandler (*createWithArgs) (const CWT_CHAR *separator, size_t max_tokens);
	void          (*free)    (CwtCsvHandler);
	void          (*write)   (CwtCsvHandler, CwtChannel*, const CWT_CHAR* argv[], size_t argc);
	BOOL          (*parse)   (CwtCsvHandler, CwtChannel*);
	void          (*setOnRow)   (CwtCsvHandler, CwtCsvCallback cb);
	void          (*error)   (CwtCsvHandler, const CWT_CHAR *errstr);
	void          (*setOnError) (CwtCsvHandler, void (*callback)(CwtCsvHandler, const CWT_CHAR*));
	size_t        (*line)    (CwtCsvHandler);

	/* Simple API for CSV (SAC) */
	void          (*begin)   (CwtCsvHandler, CwtChannel*);
	size_t        (*header)  (CwtCsvHandler);
	void          (*titles)  (CwtCsvHandler, CWT_CHAR* argv[], size_t argc);
	BOOL          (*next)    (CwtCsvHandler);
	size_t        (*columnsCount) (CwtCsvHandler);
	size_t        (*row)     (CwtCsvHandler, CWT_CHAR* argv[], size_t argc);
	CWT_CHAR*     (*column)  (CwtCsvHandler, const CWT_CHAR* name);
} CwtCsvNS;


EXTERN_C_BEGIN
DLL_API_EXPORT CwtCsvNS* cwtCsvNS(void);
EXTERN_C_END




#endif /* __CWT_CSV_H__ */
