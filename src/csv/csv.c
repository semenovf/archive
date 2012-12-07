/*
 * ini.c
 *
 *  Created on: 05.12.2011
 *      Author: wladt
 */

#include <cwt/csv/csv.h>
#include <string.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/io/channel.h>
#include <cwt/hashtab.h>


static CWT_CHAR *__csv_default_separator = _T(",");


typedef struct _CsvColumn {
	size_t index;
	BOOL (*validate)(const CWT_CHAR *val);
} CsvColumn;


/* Temporary data for row-by-row parsing */
typedef struct _CsvData {
	CwtByteArray   *ba;
	CwtStrList     *tokens;
	CwtHashTable   *columns;
	CwtChannel     *pchan;
} CsvData;

typedef struct _CwtCsvHandlerImpl {
	CWT_CHAR      *separator;
	size_t         max_tokens; /* maximum number of tokens in CSV file */
	size_t         line;
	CwtCsvCallback on_row;
	void           (*on_error)(CwtCsvContext*, const CWT_CHAR*);
	CsvData        csvData;
} CwtCsvHandlerImpl;


static CwtCsvContext*  csv_create   (void);
static CwtCsvContext*  csv_create_with_args (const CWT_CHAR *separator, size_t max_tokens);
static void            csv_free     (CwtCsvContext*);
static void            csv_write    (CwtCsvContext*, CwtChannel*, const CWT_CHAR *argv[], size_t argc);
static BOOL            csv_parse    (CwtCsvContext*, CwtChannel*);
static void            csv_set_on_row (CwtCsvContext*, CwtCsvCallback cb);
static void            csv_error    (CwtCsvContext*, const CWT_CHAR *errstr);
static void            csv_set_on_error (CwtCsvContext*, void (*callback)(CwtCsvContext*, const CWT_CHAR*));
static size_t          csv_line     (CwtCsvContext*);

/* Simple API for CSV (SAC) */
static void            csv_begin   (CwtCsvContext*, CwtChannel*);
static size_t          csv_header  (CwtCsvContext*);
static void            csv_titles  (CwtCsvContext*, CWT_CHAR **argv, size_t argc);
static BOOL            csv_next    (CwtCsvContext*);
static size_t          csv_columns_count (CwtCsvContext*);
static size_t          csv_row     (CwtCsvContext*, const CWT_CHAR **argv, size_t argc);
static const CWT_CHAR* csv_column  (CwtCsvContext*, const CWT_CHAR *name);

static BOOL            csv_persist (CwtCsvContext*, CwtDBHandler *dbh, CwtDDITable *table);
static BOOL            csv_set_validator (CwtCsvContext*, const CWT_CHAR *name, BOOL (*)(const CWT_CHAR*));

/* helper functions */
static void            destroy_csv_data(CsvData*);

static CwtCsvNS __cwtCsvNS = {
	  csv_create
	, csv_create_with_args
	, csv_free
	, csv_write
	, csv_parse
	, csv_set_on_row
	, csv_error
	, csv_set_on_error
	, csv_line

	, csv_begin
	, csv_header
	, csv_titles
	, csv_next
	, csv_columns_count
	, csv_row
	, csv_column

	, csv_persist

	, csv_set_validator
};

static CwtStrNS       *__strNS    = NULL;
static CwtStringNS    *__stringNS = NULL;
static CwtStrListNS   *__slNS     = NULL;
static CwtChannelNS   *__chNS     = NULL;
static CwtByteArrayNS *__baNS     = NULL;
static CwtTextCodecNS *__codecNS  = NULL;
static CwtHashTableNS *__htNS     = NULL;


DLL_API_EXPORT CwtCsvNS* cwt_csv_ns(void)
{
	if( ! __strNS ) {
		__strNS    = cwt_str_ns();
		__stringNS = cwt_string_ns();
		__slNS     = cwt_strList_ns();
		__chNS     = cwt_channel_ns();
		__baNS     = cwt_bytearray_ns();
		__codecNS  = cwt_textcodec_ns();
		__htNS     = cwt_hashtable_ns();
	}

	return &__cwtCsvNS;
}


static void destroy_csv_data(CsvData *data)
{
	if( !data )
		return;

	if( data->ba ) {
		__baNS->free(data->ba);
		data->ba = NULL;
	}

	if( data->tokens ) {
		__slNS->free(data->tokens);
		data->tokens = NULL;
	}

	if( data->columns ) {
		__htNS->free(data->columns);
		data->columns = NULL;
	}
}

static CwtCsvContext* csv_create(void)
{
	return csv_create_with_args(__csv_default_separator, 0);
}

static CwtCsvContext* csv_create_with_args(const CWT_CHAR *separator, size_t max_tokens)
{
	CwtCsvHandlerImpl *h;
	h = CWT_MALLOC(CwtCsvHandlerImpl);

	if( separator ) {
		h->separator = __strNS->strDup(separator);
	} else {
		h->separator = __strNS->strDup(__csv_default_separator);
	}
	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->line       = 0;
	h->on_error   = NULL;
	cwt_bzero(&h->csvData, sizeof(CsvData));
	/*h->csvData    = NULL;*/

	return (CwtCsvContext*)h;
}


static void csv_free(CwtCsvContext *h)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	if( h ) {
		if( ph->separator ) {
			CWT_FREE(ph->separator);
			ph->separator = NULL;
		}

		destroy_csv_data(&ph->csvData);
		CWT_FREE(ph);
	}
}

static void csv_write(CwtCsvContext *h, CwtChannel *pchan, const CWT_CHAR* argv[], size_t argc)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	CwtString         *s;
	size_t i;

	CWT_ASSERT(ph->separator);

	s = __stringNS->create();

	if( argc > 0 )
		__stringNS->append(s, argv[0]);

	for( i = 1; i < argc; i++ ) {
		__stringNS->append(s, ph->separator);
		__stringNS->append(s, argv[i]);
		__stringNS->appendChar(s, _T('\n'));
	}

	if( __stringNS->length(s) > 0 ) {
		char *utf8;
		utf8 = __codecNS->toUtf8(__stringNS->cstr(s), __stringNS->length(s));
		__chNS->write(pchan, (BYTE*)utf8, strlen(utf8));
		CWT_FREE(utf8);
	}


	__stringNS->free(s);
}

static BOOL csv_parse(CwtCsvContext *h, CwtChannel *pchan)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	CwtByteArray   *ba;
	CwtStrList     *tokens;
	BOOL esc = FALSE;
	BOOL ok = TRUE;
	BOOL quoted = FALSE; /* line has unbalanced quote and so continued at next line */

	CWT_ASSERT(h);

	ba = __baNS->create();
	tokens = __slNS->create();
	ph->line = 0;

	while( !__chNS->atEnd(pchan) && __chNS->readLine(pchan, ba) ) {
		CWT_CHAR *str;

		quoted = FALSE;
		ph->line++;

		/* empty line */
		if( __baNS->size(ba) == 0 )
			continue;

		if( __baNS->last(ba) == '\\' ) {
			__baNS->resize(ba, __baNS->size(ba)-1); /* remove backslash and insert space instead of it */
			__baNS->appendElem(ba, ' ');
			esc = TRUE;
			continue;
		} else {
			esc = FALSE;
		}

		/*__baNS->trim(ba);*/

		if( __baNS->size(ba) > 0 ) {
			str = __codecNS->fromUtf8(__baNS->cstr(ba), __baNS->size(ba));

			if( str && __strNS->strLen(str) > 0 ) {

				int rc = __slNS->split(tokens, str, ph->separator, CWT_QUOTES_BOTH
						, CWT_TRIM_WHITESPACES | CWT_STRIP_BOUNDING_QUOTES);

				if( rc > 0 ) {
					CWT_CHAR **argv;
					size_t argc = 0;

					argc = __slNS->size(tokens);

					if( argc <= ph->max_tokens ) {
						argv = CWT_MALLOCA(CWT_CHAR*, argc);

						__slNS->toArray(tokens, argv, &argc);

						if( ph->on_row ) {
							ok = ph->on_row(h, (const CWT_CHAR**)argv, argc);
						}
					} else {
						csv_error(h, _Tr("maximum tokens in row are exceeded"));
						ok = FALSE;
					}

					CWT_FREE(argv);
				} else if( rc < 0 ) { /* quotes are unbalanced */
					/*__csv_error(h, _Tr("quotes are unbalanced"));*/
					/*ok = FALSE;*/
					quoted = TRUE;
				}
			}
		}
		CWT_FREE(str);

		if( quoted ) {
			__baNS->appendElem(ba, '\n');
		} else {
			__baNS->clear(ba);
		}

		__slNS->clear(tokens);

		if( !ok )
			break;
	}

	__slNS->free(tokens);
	__baNS->free(ba);

	if( quoted ) {
		csv_error(h, _Tr("quotes are unbalanced"));
		ok = FALSE;
	}

	if( esc ) {
		__cwtCsvNS.error(h, _T("unexpected end-of-file"));
		ok = FALSE;
	}

	return ok;
}


static void csv_set_on_row(CwtCsvContext *h, CwtCsvCallback cb)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	ph->on_row = cb;
}


static void csv_error(CwtCsvContext *h, const CWT_CHAR *errstr)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);

	if( ph->on_error )
		ph->on_error(h, errstr);
}

static void csv_set_on_error(CwtCsvContext *h, void (*callback)(CwtCsvContext*, const CWT_CHAR*))
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);
	ph->on_error = callback;
}

static size_t csv_line(CwtCsvContext *h)
{
	CWT_ASSERT(h);
	return ((CwtCsvHandlerImpl*)h)->line;
}


static void csv_begin(CwtCsvContext *h, CwtChannel *pchan)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);

	destroy_csv_data(&ph->csvData);

	ph->csvData.ba      = __baNS->create();
	ph->csvData.tokens  = __slNS->create();
	ph->csvData.pchan   = pchan;
	ph->line            = 0;

	ph->csvData.columns = __htNS->create(__htNS->strHash, __htNS->streq, cwtFree, cwtFree);
}

static size_t csv_header(CwtCsvContext *h)
{
	if( csv_next(h) ) {
		CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
		CwtStrListIterator it;
		size_t i = 0;

		__slNS->begin(ph->csvData.tokens, &it);

		while( __slNS->hasMore(&it) ) {
			CsvColumn *column = CWT_MALLOC(CsvColumn);
			CWT_CHAR *key = __strNS->strDup(__slNS->next(&it));

			column->index = i++;
			column->validate = NULL;

			CWT_ASSERT(__htNS->insert(ph->csvData.columns, key, column));
		}
		return __slNS->size(ph->csvData.tokens);
	}
	return (size_t)0;
}




/**
 * @fn void CwtCsvNS::titles(CwtCsvHandler*, const CWT_CHAR* argv[], size_t argc);
 *
 * @param h CSV descriptor
 * @param argv
 * @param argc
 *
 * @code
 * CwtCsvNS      *csvNS = cwt_csv_ns();
 * CwtCsvHandler *csv;
 * size_t         ncolumns;
 * CWT_CHAR     **titles;
 * ...
 * ncolumns = csvNS->header(csv);
 * titles = CWT_MALLOCA(CWT_CHAR*, ncolumns);
 * csvNS->titles(csv, titles, ncolumns);
 * ...
 * CWT_FREE(titles);
 * @endcode
 */
static void csv_titles(CwtCsvContext *h, CWT_CHAR **argv, size_t argc)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	size_t count;
	size_t i = 0;

	CWT_ASSERT(h);

	count = __htNS->size(ph->csvData.columns);

	if( argv && count > 0 ) {
		CwtHashTableIterator it;

		__htNS->begin(ph->csvData.columns, &it);

		while( __htNS->hasMore(&it) && i < argc ) {
			argv[i++] = (CWT_CHAR*)__htNS->next(&it)->key;
		}
	}
}

static BOOL csv_next(CwtCsvContext *h)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	BOOL esc = FALSE;
	BOOL ok = TRUE;
	BOOL quoted = FALSE; /* line has unbalanced quote and so continued at next line */

	CWT_ASSERT(h);

	__slNS->clear(ph->csvData.tokens);
	__baNS->clear(ph->csvData.ba);

	if( __chNS->atEnd(ph->csvData.pchan) )
		return FALSE;


	while( !__chNS->atEnd(ph->csvData.pchan) && __chNS->readLine(ph->csvData.pchan, ph->csvData.ba) ) {
		CWT_CHAR *str;

		quoted = FALSE;
		ph->line++;

		/* empty line */
		if( __baNS->size(ph->csvData.ba) == 0 )
			continue;

		if( __baNS->last(ph->csvData.ba) == '\\' ) {
			__baNS->resize(ph->csvData.ba, __baNS->size(ph->csvData.ba)-1); /* remove backslash and insert space instead of it */
			__baNS->appendElem(ph->csvData.ba, ' ');
			esc = TRUE;
			continue;
		} else {
			esc = FALSE;
		}

		if( __baNS->size(ph->csvData.ba) > 0 ) {

			/* TODO need apply text codec insteed of fromUtf8 call */
			str = __codecNS->fromUtf8(__baNS->cstr(ph->csvData.ba), __baNS->size(ph->csvData.ba));

			if( str && __strNS->strLen(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = __slNS->split(ph->csvData.tokens, str, ph->separator, CWT_QUOTES_BOTH
							, CWT_TRIM_WHITESPACES | CWT_STRIP_BOUNDING_QUOTES);

					if( rc > 0 ) {
						if( __slNS->size(ph->csvData.tokens) > ph->max_tokens ) {
							csv_error(h, _Tr("maximum tokens in row are exceeded"));
							ok = FALSE;
						}
					} else if( rc < 0 ) { /* quotes are unbalanced */
						quoted = TRUE;
					}
				}
			}
		}
		CWT_FREE(str);

		if( quoted ) {
			__baNS->appendElem(ph->csvData.ba, '\n');
			continue;
		}

		break;
	}

	if( ok ) {
		if( quoted ) {
			csv_error(h, _Tr("quotes are unbalanced"));
			ok = FALSE;
		}

		if( esc ) {
			__cwtCsvNS.error(h, _T("unexpected end-of-file"));
			ok = FALSE;
		}
	}

	return ok;
}


/**
 * @fn BOOL CwtCsvNS::columnsCount(CwtCsvHandler *h)
 * @brief Returns number of columns (fields) in the row
 *
 * @param h CSV handler
 * @return number of columns (fields) in the row
 */
static size_t csv_columns_count(CwtCsvContext *h)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	CWT_ASSERT(h);
	return __slNS->size(ph->csvData.tokens);
}


/**
 *
 * @param h CSV descriptor.
 * @param argv
 * @param argc
 * @return actual Number of columns.
 */
static size_t csv_row(CwtCsvContext *h, const CWT_CHAR **argv, size_t argc)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	size_t count;
	size_t i;
	CwtStrListIterator it;

	CWT_ASSERT(h);

	count = CWT_MIN(__slNS->size(ph->csvData.tokens), argc);

	if( argv && count > 0 ) {
		__slNS->begin(ph->csvData.tokens, &it);
		i = 0;

		while( count > 0 && __slNS->hasMore(&it) ) {
			argv[i++] = __slNS->next(&it);
			count--;
		}
	}

	return __slNS->size(ph->csvData.tokens);
}


/**
 * @fn CWT_CHAR* CwtCsvNS::column(CwtCsvHandler *h, const CWT_CHAR* name)
 *
 * @brief Returns value of the column specified by name @c name or NULL if column not found.
 *
 * @param h CSV descriptor.
 * @param name Name of the column.
 * @return Value of the column specified by name @c name or NULL if column not found.
 */
static const CWT_CHAR* csv_column(CwtCsvContext *h, const CWT_CHAR* name)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	CsvColumn  *column;
	CwtString  *tmpbuf = NULL;
	const CWT_CHAR *val = NULL;

	CWT_ASSERT(h);

	column = __htNS->lookup(ph->csvData.columns, (void*)name);


	if( column && column->index < __slNS->size(ph->csvData.tokens) ) {
		val = __slNS->at(ph->csvData.tokens, column->index);

		if( column->validate ) {
			if( !column->validate(val) ) {
				tmpbuf = __stringNS->create();
				__stringNS->sprintf(tmpbuf, _Tr("'%s': bad value"), name);
				__cwtCsvNS.error(h, __stringNS->cstr(tmpbuf));
				val = NULL;
			}
		}
	} else {
		tmpbuf = __stringNS->create();
		__stringNS->sprintf(tmpbuf, _Tr("'%s': invalid column specified"), name);
		__cwtCsvNS.error(h, __stringNS->cstr(tmpbuf));
	}

	if( tmpbuf )
		__stringNS->free(tmpbuf);

	return val;
}

static BOOL csv_persist (CwtCsvContext *csv, CwtDBHandler *dbh, CwtDDITable *table)
{
	CwtUniTypeNS *utNS = cwt_unitype_ns();
	CwtDBI       *dbi  = cwt_dbi_ns();

	BOOL ok = TRUE;
	size_t ncolumns;
	CwtString    *tmpbuf = NULL;
	CWT_CHAR    **titles;
	CwtString    *sql;
	CwtStatement *sth;
	CwtUniType  **uts;
	size_t i;

	ncolumns = __cwtCsvNS.header(csv);

	if( !ncolumns )
		return FALSE;

	titles = CWT_MALLOCA(CWT_CHAR*, ncolumns);
	uts    = CWT_MALLOCA(CwtUniType*, ncolumns);
	tmpbuf = __stringNS->create();

	__cwtCsvNS.titles(csv, titles, ncolumns);
	sql = __stringNS->create();

	/* Prepare statement for INSERT VALUES */
	__stringNS->append(sql, _T("INSERT INTO "));
	__stringNS->append(sql, table->name);
	__stringNS->appendChar(sql, _T('('));
	__stringNS->append(sql, titles[0]);
	for( i = 1; i < ncolumns; i++ ) {
		__stringNS->appendChar(sql, _T(','));
		__stringNS->append(sql, titles[i]);
	}

	__stringNS->append(sql, _T(") VALUES("));
	__stringNS->appendChar(sql, _T('?'));
	for( i = 1; i < ncolumns; i++ ) {
		__stringNS->append(sql, _T(",?"));
	}
	__stringNS->appendChar(sql, _T(')'));

	sth = dbi->prepare(dbh, __stringNS->cstr(sql));

	if( !sth ) {
		ok = FALSE;
	} else {
		CwtUniType *ut = utNS->create();

		/* Bind values */
		for( i = 0; i < ncolumns; i++ ) {
			CwtDDIColumn   *col;
			col = dbi->findColumn(table, titles[i]);

			if( !col ) {
				__stringNS->sprintf(tmpbuf, _Tr("'%s': column not found"), titles[i]);
				__cwtCsvNS.error(csv, __stringNS->cstr(tmpbuf));
				ok = FALSE;
				break;
			}

			if(col->type == CwtType_TEXT)
				uts[i] = dbi->bindTextByIndex(sth, i, 256);
			else if(col->type == CwtType_BLOB)
				uts[i] = dbi->bindBlobByIndex(sth, i, 256);
			else
				uts[i] = dbi->bindByIndex(sth, i, col->type);
		}


		while( ok && __cwtCsvNS.next(csv) ) {
			if( __cwtCsvNS.columnsCount(csv) != ncolumns ) {
				__cwtCsvNS.error(csv, _T("invalid number of columns"));
				ok = FALSE;
				break;
			}

			for( i = 0; i < ncolumns; i++ ) {
				const CWT_CHAR *value;

				value = __cwtCsvNS.column(csv, titles[i]);

				ok = utNS->setFromString(ut, uts[i]->type, value);
				if( ok ) {
					ok = dbi->setUniType(sth, uts[i], ut);
				}

				if( !ok ) {
					__stringNS->sprintf(tmpbuf, _Tr("invalid value for column '%s'"), titles[i]);
					__cwtCsvNS.error(csv, __stringNS->cstr(tmpbuf));
					break;
				}
			}

			if( ok )
				ok = dbh->execute(sth);
		}

		utNS->free(ut);
	}

	__stringNS->free(sql);
	__stringNS->free(tmpbuf);
	CWT_FREE(titles);
	CWT_FREE(uts);
	dbi->close(sth);

	return ok;
}


static BOOL csv_set_validator (CwtCsvContext *h, const CWT_CHAR* name, BOOL (*validator)(const CWT_CHAR*))
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	CsvColumn  *column;

	CWT_ASSERT(h);

	column = __htNS->lookup(ph->csvData.columns, (void*)name);

	if( column && column->index < __slNS->size(ph->csvData.tokens) ) {
		column->validate = validator;
		return TRUE;
	} else {
		CwtString *tmpbuf = __stringNS->create();
		__stringNS->sprintf(tmpbuf, _Tr("'%s': invalid column specified"), name);
		__cwtCsvNS.error(h, __stringNS->cstr(tmpbuf));
		__stringNS->free(tmpbuf);
	}

	return FALSE;
}
