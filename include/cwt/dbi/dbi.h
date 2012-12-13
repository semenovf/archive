/*
 * dbi.h
 *
 *  Created on: 03.07.2012
 *      Author: wladt
 *
 * Database interface based on Perl's DBI API subset
 *
 */

#ifndef __CWT_DBI_H__
#define __CWT_DBI_H__

#include <cwt/types.h>
#include <cwt/unitype.h>
#include <cwt/strlist.h>
#include <cwt/dbi/ddi.h>

typedef INT32 CwtDBI_RC;

typedef enum CwtSqlTypeEnum {
	  CwtSql_TINYINT
	, CwtSql_SMALLINT
	, CwtSql_MEDIUMINT
	, CwtSql_INT
	, CwtSql_BIGINT
	, CwtSql_FLOAT
	, CwtSql_DOUBLE
	, CwtSql_DECIMAL
	, CwtSql_YEAR
	, CwtSql_TIME
	, CwtSql_DATE
	, CwtSql_DATETIME
	, CwtSql_TIMESTAMP
	, CwtSql_CHAR
	, CwtSql_BINARY
	, CwtSql_VARCHAR
	, CwtSql_VARBINARY
	, CwtSql_TINYBLOB
	, CwtSql_TINYTEXT
	, CwtSql_BLOB
	, CwtSql_TEXT
	, CwtSql_MEDIUMBLOB
	, CwtSql_MEDIUMTEXT
	, CwtSql_LONGBLOB
	, CwtSql_LONGTEXT
	, CwtSql_BIT
} CwtSqlTypeEnum;


struct _CwtDBIDriver;
struct _CwtDBHandler;

typedef struct CwtStatement {
	struct _CwtDBHandler *dbh;
	CwtUniType          **bind_params;
} CwtStatement;

typedef struct _CwtDBHandler {
	void            (*close)         (CwtStatement*);
	BOOL            (*execute)       (CwtStatement*);
	ULONGLONG       (*lastId)        (CwtStatement*);
	CwtDBI_RC       (*err)           (CwtStatement*);
	const CWT_CHAR* (*strerror)      (CwtStatement*);
	BOOL            (*bindByIndex)   (CwtStatement*, size_t index, CwtUniType *ut);
	size_t          (*bindParmsCount)(CwtStatement*);
	BOOL            (*setParm)       (CwtStatement*, CwtUniType *ut, const void *copy, size_t sz);
	ULONGLONG       (*rows)          (CwtStatement*);
	ULONGLONG       (*size)          (CwtStatement*);
	BOOL            (*fetchNext)     (CwtStatement*);
	BOOL            (*fetchColumn)   (CwtStatement*, CWT_CHAR *col, CwtUniType *value);
	struct _CwtDBIDriver* (*driver)  (void);
} CwtDBHandler;

typedef struct _CwtDBIDriver
{
	CwtDBHandler*   (*connect)       (const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
	void            (*disconnect)    (CwtDBHandler*);
	BOOL            (*func)          (CwtDBHandler*, const CWT_CHAR*, CWT_CHAR**);
	void            (*attr)          (CwtDBHandler*, const CWT_CHAR*, void*);
	BOOL            (*setAutoCommit) (CwtDBHandler*, BOOL);
	BOOL            (*autoCommit)    (CwtDBHandler*);
	CwtDBI_RC       (*err)           (CwtDBHandler*);
	const CWT_CHAR* (*strerror)      (CwtDBHandler*);
	const CWT_CHAR* (*state)         (CwtDBHandler*);
	BOOL            (*query)         (CwtDBHandler*, const CWT_CHAR *sql);   /* cannot be used for statements that contain binary data */
	/*BOOL            (*queryBin)      (CwtDBHandler*, const CWT_CHAR *sql, size_t length); *//* can be used for statements that contain binary data */
	CwtStatement*   (*prepare)       (CwtDBHandler*, const CWT_CHAR *sql);
	ULONGLONG       (*rows)          (CwtDBHandler*);
	BOOL            (*tables)        (CwtDBHandler*, CwtStrList *tables);
	BOOL            (*tableExists)   (CwtDBHandler*, const CWT_CHAR *tname);
	char*           (*encode_n)      (CwtDBHandler*, const CWT_CHAR *s, size_t n);
	CWT_CHAR*       (*decode_n)      (CwtDBHandler*, const char *s, size_t n);
	char*           (*encode)        (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)        (CwtDBHandler*, const char *s);
	BOOL            (*begin)         (CwtDBHandler*); /* begin transaction */
	BOOL            (*commit)        (CwtDBHandler*);
	BOOL            (*rollback)      (CwtDBHandler*);

	CwtStrList*     (*specForDeploy) (CwtDDI*, int flags);
	CwtStrList*     (*specForRecall) (CwtDDI*, int flags);
} CwtDBIDriver;


/*
typedef struct _CwtDBIBindGroup {

} CwtDBIBindGroup;
*/

typedef struct CwtDBI
{
	void            (*parseDSN)       (const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN);
	CwtDBIDriver*   (*load)           (const CWT_CHAR *scheme, const CWT_CHAR *driver);
	CwtDBHandler*   (*connect)        (const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
	void            (*disconnect)     (CwtDBHandler*);
	BOOL            (*func)           (CwtDBHandler*, const CWT_CHAR*, CWT_CHAR**);
	void            (*attr)           (CwtDBHandler*, const CWT_CHAR*, void*);
	BOOL            (*setAutoCommit)  (CwtDBHandler*, BOOL);
	BOOL            (*autoCommit)     (CwtDBHandler*);
	CwtDBI_RC       (*err)            (CwtDBHandler*);
	const CWT_CHAR* (*strerror)       (CwtDBHandler*);
	const CWT_CHAR* (*state)          (CwtDBHandler*);
	BOOL            (*query)          (CwtDBHandler*, const CWT_CHAR *sql);   /* cannot be used for statements that contain binary data */
	CwtStatement*   (*prepare)        (CwtDBHandler*, const CWT_CHAR *sql);
	BOOL            (*execute)        (CwtStatement*);
	ULONGLONG       (*lastId)         (CwtStatement*);
	ULONGLONG       (*rows)           (CwtDBHandler*);
	BOOL            (*tables)         (CwtDBHandler*, CwtStrList *tables);
	BOOL            (*tableExists)    (CwtDBHandler*, const CWT_CHAR *tname);
	char*           (*encode_n)       (CwtDBHandler*, const CWT_CHAR *s, size_t n);
	CWT_CHAR*       (*decode_n)       (CwtDBHandler*, const char *s, size_t n);
	char*           (*encode)         (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)         (CwtDBHandler*, const char *s);
	BOOL            (*begin)          (CwtDBHandler*); /* begin transaction */
	BOOL            (*commit)         (CwtDBHandler*);
	BOOL            (*rollback)       (CwtDBHandler*);

	void            (*close)          (CwtStatement *sth);
	CwtUniType*     (*bindByIndex)    (CwtStatement *sth, size_t index, CwtTypeEnum cwtType);
	CwtUniType*     (*bindTextByIndex)(CwtStatement *sth, size_t index, size_t length);
	CwtUniType*     (*bindBlobByIndex)(CwtStatement *sth, size_t index, size_t sz);
	BOOL            (*setFromString)  (CwtStatement *sth, CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
	BOOL            (*setUniType)     (CwtStatement *sth, CwtUniType *ut, CwtUniType *val);
	BOOL            (*setBOOL)        (CwtStatement *sth, CwtUniType *ut, BOOL b);
	BOOL            (*setCHAR)        (CwtStatement *sth, CwtUniType *ut, CWT_CHAR ch);
	BOOL            (*setSBYTE)       (CwtStatement *sth, CwtUniType *ut, SBYTE n);
	BOOL            (*setBYTE)        (CwtStatement *sth, CwtUniType *ut, BYTE n);
	BOOL            (*setSHORT)       (CwtStatement *sth, CwtUniType *ut, SHORT n);
	BOOL            (*setUSHORT)      (CwtStatement *sth, CwtUniType *ut, USHORT n);
	BOOL            (*setINT)         (CwtStatement *sth, CwtUniType *ut, INT n);
	BOOL            (*setUINT)        (CwtStatement *sth, CwtUniType *ut, UINT n);
	BOOL            (*setLONG)        (CwtStatement *sth, CwtUniType *ut, LONG n);
	BOOL            (*setULONG)       (CwtStatement *sth, CwtUniType *ut, ULONG n);
	BOOL            (*setLONGLONG)    (CwtStatement *sth, CwtUniType *ut, LONGLONG n);
	BOOL            (*setULONGLONG)   (CwtStatement *sth, CwtUniType *ut, ULONGLONG n);
	BOOL            (*setFLOAT)       (CwtStatement *sth, CwtUniType *ut, float n);
	BOOL            (*setDOUBLE)      (CwtStatement *sth, CwtUniType *ut, double n);
	BOOL            (*setTEXT)        (CwtStatement *sth, CwtUniType *ut, const CWT_CHAR *p, size_t length);
	BOOL            (*setBLOB)        (CwtStatement *sth, CwtUniType *ut, const void *p, size_t sz);
	BOOL            (*setTIME)        (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p);
	BOOL            (*setDATE)        (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p);
	BOOL            (*setDATETIME)    (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p);

	BOOL            (*fetchNext)      (CwtStatement*);
	BOOL            (*fetchColumn)    (CwtStatement*, CWT_CHAR *col, CwtUniType *ut);

	CwtTypeEnum     (*toCwtTypeEnum)  (CwtSqlTypeEnum sqlType);
	CwtSqlTypeEnum  (*toSqlTypeEnum)  (CwtTypeEnum cwtType);

	/* DDI specific methods */
	CwtDDI*         (*createDDI)      (const CWT_CHAR *name, const CWT_CHAR *charset);
	CwtDDI*         (*buildDDI)       (const CWT_CHAR *name, const CWT_CHAR *charset, CwtPersistEntity *entities[], size_t nentries);
	void            (*freeDDI)        (CwtDDI*);

	CwtDDITable*    (*newTable)       (CwtDDI*, const CWT_CHAR *name);
	CwtDDIColumn*   (*newColumn)      (CwtDDITable*, const CWT_CHAR *name);
	CwtDDITable*    (*findTable)      (CwtDDI*, const CWT_CHAR *name);
	CwtDDIColumn*   (*findColumn)     (CwtDDITable*, const CWT_CHAR *name);

	BOOL            (*setColumnTypeBool)    (CwtDDIColumn*);
	BOOL            (*setColumnTypeChar)    (CwtDDIColumn*);
	BOOL            (*setColumnTypeInt)     (CwtDDIColumn*, LONGLONG min, ULONGLONG max);
	BOOL            (*setColumnTypeFloat)   (CwtDDIColumn*);
	BOOL            (*setColumnTypeDouble)  (CwtDDIColumn*);
	BOOL            (*setColumnTypeDecimal) (CwtDDIColumn*, UINT prec, UINT scale);
	BOOL            (*setColumnTypeText)    (CwtDDIColumn*, ULONGLONG maxlen);
	BOOL            (*setColumnTypeBlob)    (CwtDDIColumn*, ULONGLONG maxlen);
	BOOL            (*setColumnTypeTime)    (CwtDDIColumn*, CwtTypeEnum time_type, BOOL stamp);
	BOOL            (*setColumnTypeRef)     (CwtDDIColumn*, CwtDDITable*);
	BOOL            (*setColumnDefault)     (CwtDDIColumn*, const CWT_CHAR *defaultValue);
	BOOL            (*setColumnAutoinc)     (CwtDDIColumn*, UINT inc);
	BOOL            (*setColumnNullable)    (CwtDDIColumn*, BOOL yes);
	BOOL            (*setColumnIndexable)   (CwtDDIColumn*, BOOL yes);
	BOOL            (*setColumnUnique)      (CwtDDIColumn*, BOOL yes);
	BOOL            (*setColumnPK)          (CwtDDIColumn*);

	BOOL            (*deploy)         (CwtDBHandler*, CwtDDI *ddi, int flags);
	BOOL            (*recall)         (CwtDBHandler*, CwtDDI *ddi, int flags);
} CwtDBI;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtDBI* cwt_dbi_ns(void);
EXTERN_C_END

#endif /* __CWT_DBI_H__ */
