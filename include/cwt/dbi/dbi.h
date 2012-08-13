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

/*
typedef struct CwtBindEntry {
	CwtTypeId type_id;
	void     *value;
	size_t   *plength;
} CwtBindEntry;
*/

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
	CwtDBI_RC       (*err)           (CwtStatement*);
	const CWT_CHAR* (*strerror)      (CwtStatement*);
	BOOL            (*bind)          (CwtStatement *sth, size_t index, CwtUniType *ut);
	size_t          (*bindParamsCount) (CwtStatement *sth);
	ULONGLONG       (*rows)          (CwtStatement*);
	ULONGLONG       (*size)          (CwtStatement*);
	BOOL            (*fetchNext)     (CwtStatement*);
	BOOL            (*fetchColumn)   (CwtStatement*, CWT_CHAR *col, CwtUniType *value);
	struct _CwtDBIDriver* (*driver)   (void);
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
	BOOL            (*queryBin)      (CwtDBHandler*, const CWT_CHAR *sql, size_t length); /* can be used for statements that contain binary data */
	CwtStatement*   (*prepare)       (CwtDBHandler*, const CWT_CHAR *sql);
	ULONGLONG       (*rows)          (CwtDBHandler*);
	BOOL            (*tables)        (CwtDBHandler*, CwtStrList *tables);
	BOOL            (*tableExists)   (CwtDBHandler*, const CWT_CHAR *tname);
	char*           (*encode)        (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)        (CwtDBHandler*, const char *s);
	CWT_TIME*       (*createTime)    (void);
	void            (*freeTime)      (CWT_TIME*);
	void            (*convertTime)   (CWT_TIME *cwtTime, void *nativeTime);
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
	BOOL            (*queryBin)      (CwtDBHandler*, const CWT_CHAR *sql, size_t length); /* can be used for statements that contain binary data */
	CwtStatement*   (*prepare)       (CwtDBHandler*, const CWT_CHAR *sql);
	ULONGLONG       (*rows)          (CwtDBHandler*);
	BOOL            (*tables)        (CwtDBHandler*, CwtStrList *tables);
	BOOL            (*tableExists)   (CwtDBHandler*, const CWT_CHAR *tname);
	char*           (*encode)        (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)        (CwtDBHandler*, const char *s);
	BOOL            (*begin)         (CwtDBHandler*); /* begin transaction */
	BOOL            (*commit)        (CwtDBHandler*);
	BOOL            (*rollback)      (CwtDBHandler*);

	void            (*close)         (CwtStatement *sth);
	CwtUniType*     (*bind)          (CwtStatement *sth, size_t index, CwtTypeEnum cwtType);
	CwtUniType*     (*bindText)      (CwtStatement *sth, size_t index, size_t length);
	CwtUniType*     (*bindBlob)      (CwtStatement *sth, size_t index, size_t sz);

/*
	CwtDBIBindGroup* (*createBindGroup)(void);
	void             (*freeBindGroup)(CwtDBIBindGroup*);
	void             (*addBind)      (CwtDBIBindGroup*, CwtUniType *ut);
*/


	CwtTypeEnum     (*toCwtTypeEnum) (CwtSqlTypeEnum sqlType);
	CwtSqlTypeEnum  (*toSqlTypeEnum) (CwtTypeEnum cwtType);

	/* DDI specific methods */
	CwtDDI*         (*createDDI)     (const CWT_CHAR *name, const CWT_CHAR *charset);
	void            (*freeDDI)       (CwtDDI*);
	CwtDDITable*    (*newTable)      (CwtDDI*, const CWT_CHAR *name);
	CwtDDIColumn*   (*newColumn)     (CwtDDITable*, const CWT_CHAR *name);
	CwtDDITable*    (*findTable)      (CwtDDI*, const CWT_CHAR *name);
	CwtDDIColumn*   (*findColumn)     (CwtDDITable*, const CWT_CHAR *name);

	BOOL            (*cTypeBool)     (CwtDDIColumn*);
	BOOL            (*cTypeInt)      (CwtDDIColumn*, LONGLONG min, ULONGLONG max);
	BOOL            (*cTypeFloat)    (CwtDDIColumn*, UINT prec, UINT scale);
	BOOL            (*cTypeText)     (CwtDDIColumn*, ULONGLONG maxlen);
	BOOL            (*cTypeBlob)     (CwtDDIColumn*, ULONGLONG maxlen);
	BOOL            (*cTypeTime)     (CwtDDIColumn*, CwtTypeEnum time_type, BOOL stamp);
	BOOL            (*cTypeRef)      (CwtDDIColumn*, CwtDDITable*);
	BOOL            (*cDefault)      (CwtDDIColumn*, const CWT_CHAR *defaultValue);
	BOOL            (*cAutoinc)      (CwtDDIColumn*, UINT inc);
	BOOL            (*cNull)         (CwtDDIColumn*, BOOL yes);
	BOOL            (*cIndex)        (CwtDDIColumn*, BOOL yes);
	BOOL            (*cUniq)         (CwtDDIColumn*, BOOL yes);
	BOOL            (*cPK)           (CwtDDIColumn*);

	BOOL            (*deploy)        (CwtDBHandler*, CwtDDI *ddi, int flags);
	BOOL            (*recall)        (CwtDBHandler*, CwtDDI *ddi, int flags);
} CwtDBI;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtDBI* cwtDBI(void);
EXTERN_C_END

#endif /* __CWT_DBI_H__ */
