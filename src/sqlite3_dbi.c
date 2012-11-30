/**
 * @file   sqlite3_dbi.c
 * @author wladt
 * @date   Nov 28, 2012 9:59:57 AM
 *
 * @brief
 */


#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
/*#include <cwt/hashtab.h>*/
/*
#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
*/


#define __LOG_PREFIX _T("sqlite3: ")
/*
#define __DBH(dbh)  (((CwtMySqlDBHandler*)(dbh))->conn)
#define __STH(sth)  (((CwtMySqlStatement*)(sth))->stmt)
*/

typedef struct _CwtSqliteStatement {
	CwtStatement __base;
} CwtSqliteStatement;


typedef struct _CwtSqliteDBHandler
{
	CwtDBHandler __base;
} CwtSqliteDBHandler;


/*
typedef struct _CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;
*/

static CwtStrNS       *__strNS = NULL;
/*static CwtHashTableNS *__htNS = NULL;*/
static CwtUniTypeNS   *__utNS = NULL;

/* DBI API functions implementations */
/*
CwtDBHandler*           __dbd_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void             __dbd_disconnect(CwtDBHandler *dbh);
static BOOL             __dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             __dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             __dbd_setAutoCommit(CwtDBHandler *dbh, BOOL on);
static BOOL             __dbd_autoCommit(CwtDBHandler *dbh);
static CwtDBI_RC        __dbd_err(CwtDBHandler *dbh);
static const CWT_CHAR*  __dbd_strerror(CwtDBHandler *dbh);
static const CWT_CHAR*  __dbd_state(CwtDBHandler *dbh);
static BOOL             __dbd_query(CwtDBHandler *dbh, const CWT_CHAR *sql);
static BOOL             __dbd_queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length);
static CwtStatement*    __dbd_prepare(CwtDBHandler *dbh, const CWT_CHAR *statement);
static ULONGLONG        __dbd_affectedRows(CwtDBHandler *dbh);
static BOOL             __dbd_tables(CwtDBHandler *dbh, CwtStrList *tables);
static BOOL             __dbd_tableExists(CwtDBHandler*, const CWT_CHAR *tname);
static char*            __dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n);
static CWT_CHAR*        __dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n);
static char*            __dbd_encode(CwtDBHandler *dbh, const CWT_CHAR *s) { return __dbd_encode_n(dbh, s, __strNS->strLen(s)); }
static CWT_CHAR*        __dbd_decode(CwtDBHandler *dbh, const char *s) { return __dbd_decode_n(dbh, s, strlen(s)); }
*/

/*
static BOOL             __dbd_begin(CwtDBHandler *dbh);
static BOOL             __dbd_commit(CwtDBHandler *dbh);
static BOOL             __dbd_rollback(CwtDBHandler *dbh);

extern CwtStrList*      __dbd_specForDeploy(CwtDDI *ddi, int flags);
extern CwtStrList*      __dbd_specForRecall(CwtDDI *ddi, int flags);
*/

/*
static void             __stmt_close       (CwtStatement *sth);
static BOOL             __stmt_execute     (CwtStatement *sth);
static ULONGLONG        __stmt_lastId      (CwtStatement *sth);
static ULONGLONG        __stmt_affectedRows(CwtStatement *sth);
static ULONGLONG        __stmt_numRows     (CwtStatement*);
static BOOL             __stmt_fetchNext   (CwtStatement*);
static BOOL             __stmt_fetchColumn (CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut);
static CwtDBI_RC        __stmt_err         (CwtStatement *sth);
static const CWT_CHAR*  __stmt_strerror    (CwtStatement *sth);
static BOOL             __stmt_bindByIndex (CwtStatement *sth, size_t index, CwtUniType *ut);
static inline size_t    __stmt_bindParmsCount(CwtStatement *sth) { CWT_ASSERT(sth); return ((CwtMySqlStatement*)sth)->nbind_params; }
static BOOL             __stmt_setParm     (CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz);
*/

/* local helper functions */
/*
static BOOL             __mysql_isBlob(int mysqltype);
static BOOL             __mysql_isInteger(int mysqltype);
static BOOL             __mysql_isTime(int mysqltype);
static enum enum_field_types
                        __mysql_toMysqlType(CwtTypeEnum cwtType);
static CwtTypeEnum      __mysql_fromMysqlType( enum enum_field_types mysqlType, UINT field_flags);
static BOOL             __mysql_isUnsigned(CwtTypeEnum cwtType);
static void             __mysql_destroy(CwtMySqlDBHandler *dbh);
static void             __mysql_stmtDestroy(CwtMySqlStatement *sth);
static BOOL             __mysql_buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[]);
static BOOL             __mysql_buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[]);
static int              __mysql_realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length);
*/

/*static BOOL __nConnections = 0;*/ /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  NULL /*__dbd_connect          */
	, NULL /*__dbd_disconnect       */
	, NULL /*__dbd_func             */
	, NULL /*__dbd_attr             */
	, NULL /*__dbd_setAutoCommit    */
	, NULL /*__dbd_autoCommit       */
	, NULL /*__dbd_err              */
	, NULL /*__dbd_strerror         */
	, NULL /*__dbd_state            */
	, NULL /*__dbd_query            */
	, NULL /*__dbd_queryBin         */
	, NULL /*__dbd_prepare          */
	, NULL /*__dbd_affectedRows     */
	, NULL /*__dbd_tables           */
	, NULL /*__dbd_tableExists      */
	, NULL /*__dbd_encode_n         */
	, NULL /*__dbd_decode_n         */
	, NULL /*__dbd_encode           */
	, NULL /*__dbd_decode           */
	, NULL /*__dbd_begin            */
    , NULL /*__dbd_commit           */
    , NULL /*__dbd_rollback         */

    , NULL /*__dbd_specForDeploy    */
    , NULL /*__dbd_specForRecall    */
};

DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	if( !__strNS ) {
		__strNS = cwt_str_ns();
		/*__htNS  = cwtHashTableNS();*/
		__utNS  = cwt_unitype_ns();
	}
	return &__cwtDBIDriver;
}
