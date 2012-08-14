/*
 * ini.c
 *
 *  Created on: 05.12.2011
 *      Author: wladt
 */


#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/bytearr.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/io/channel.h>
#include <cwt/csv/csv.h>
#include <cwt/hashtab.h>


static CWT_CHAR *__csv_default_separator = _T(",");


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
	void           (*on_error)(CwtCsvHandler*, const CWT_CHAR*);
	CsvData        csvData;
} CwtCsvHandlerImpl;


static CwtCsvHandler*  __csv_create   (void);
static CwtCsvHandler*  __csv_createWithArgs (const CWT_CHAR *separator, size_t max_tokens);
static void            __csv_free     (CwtCsvHandler*);
static void            __csv_write    (CwtCsvHandler*, CwtChannel*, const CWT_CHAR* argv[], size_t argc);
static BOOL            __csv_parse    (CwtCsvHandler*, CwtChannel*);
static void            __csv_setOnRow (CwtCsvHandler*, CwtCsvCallback cb);
static void            __csv_error    (CwtCsvHandler*, const CWT_CHAR *errstr);
static void            __csv_setOnError (CwtCsvHandler*, void (*callback)(CwtCsvHandler*, const CWT_CHAR*));
static size_t          __csv_line     (CwtCsvHandler*);

/* Simple API for CSV (SAC) */
static void            __csv_begin   (CwtCsvHandler*, CwtChannel*);
static size_t          __csv_header  (CwtCsvHandler*);
static void            __csv_titles  (CwtCsvHandler*, const CWT_CHAR* argv[], size_t argc);
static BOOL            __csv_next    (CwtCsvHandler*);
static size_t          __csv_columnsCount (CwtCsvHandler*);
static size_t          __csv_row     (CwtCsvHandler*, const CWT_CHAR* argv[], size_t argc);
static const CWT_CHAR* __csv_column  (CwtCsvHandler*, const CWT_CHAR* name);

/* helper functions */
static void          __csv_destroyCsvData(CsvData*);


static CwtCsvNS __cwtCsvNS = {
	  __csv_create
	, __csv_createWithArgs
	, __csv_free
	, __csv_write
	, __csv_parse
	, __csv_setOnRow
	, __csv_error
	, __csv_setOnError
	, __csv_line

	, __csv_begin
	, __csv_header
	, __csv_titles
	, __csv_next
	, __csv_columnsCount
	, __csv_row
	, __csv_column
};

static CwtStrNS       *__strNS    = NULL;
static CwtStringNS    *__stringNS = NULL;
static CwtStrListNS   *__slNS     = NULL;
static CwtChannelNS   *__chNS     = NULL;
static CwtByteArrayNS *__baNS     = NULL;
static CwtTextCodecNS *__codecNS  = NULL;
static CwtHashTableNS *__htNS     = NULL;


DLL_API_EXPORT CwtCsvNS* cwtCsvNS(void)
{
	if( ! __strNS ) {
		__strNS    = cwtStrNS();
		__stringNS = cwtStringNS();
		__slNS     = cwtStrListNS();
		__chNS     = cwtChannelNS();
		__baNS     = cwtByteArrayNS();
		__codecNS  = cwtTextCodecNS();
		__htNS     = cwtHashTableNS();
	}

	return &__cwtCsvNS;
}


static void __csv_destroyCsvData(CsvData *data)
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

static CwtCsvHandler* __csv_create(void)
{
	return __csv_createWithArgs(__csv_default_separator, 0);
}

static CwtCsvHandler* __csv_createWithArgs(const CWT_CHAR *separator, size_t max_tokens)
{
	CwtCsvHandlerImpl *h;
	h = CWT_MALLOC(CwtCsvHandlerImpl);

	if( separator ) {
		h->separator = __strNS->dup(separator);
	} else {
		h->separator = __strNS->dup(__csv_default_separator);
	}
	h->max_tokens = max_tokens > 0 ? max_tokens : 128;
	h->line       = 0;
	h->on_error   = NULL;
	__strNS->bzero(&h->csvData, sizeof(CsvData));
	/*h->csvData    = NULL;*/

	return (CwtCsvHandler*)h;
}


static void __csv_free(CwtCsvHandler *h)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	if( h ) {
		if( ph->separator ) {
			CWT_FREE(ph->separator);
			ph->separator = NULL;
		}

		__csv_destroyCsvData(&ph->csvData);
		CWT_FREE(ph);
	}
}

static void __csv_write(CwtCsvHandler *h, CwtChannel *pchan, const CWT_CHAR* argv[], size_t argc)
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

static BOOL __csv_parse(CwtCsvHandler *h, CwtChannel *pchan)
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
			str = __codecNS->fromUtf8(__baNS->cstr(ba), __baNS->size(ba)); /* TODO need apply text codec insteed of fromUtf8 call */

			if( str && __strNS->len(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = __slNS->split(tokens, str, ph->separator, CWT_QUOTES_BOTH);

					if( rc > 0 ) {
						CWT_CHAR **argv;
						size_t argc = 0;

						argc = __slNS->size(tokens);

						if( argc <= ph->max_tokens ) {
							argv = CWT_MALLOCA(CWT_CHAR*, argc);

							__slNS->toArray(tokens, argv, &argc);

							if( ph->on_row ) {
								ok = ph->on_row(h, argv, argc);
							}
						} else {
							__csv_error(h, _Tr("maximum tokens in row are exceeded"));
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
		__csv_error(h, _Tr("quotes are unbalanced"));
		ok = FALSE;
	}

	if( esc ) {
		__cwtCsvNS.error(h, _T("unexpected end-of-file"));
		ok = FALSE;
	}

	return ok;
}


static void __csv_setOnRow(CwtCsvHandler *h, CwtCsvCallback cb)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	ph->on_row = cb;
}


static void __csv_error(CwtCsvHandler *h, const CWT_CHAR *errstr)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);

	if( ph->on_error )
		ph->on_error(h, errstr);
}

static void __csv_setOnError(CwtCsvHandler *h, void (*callback)(CwtCsvHandler*, const CWT_CHAR*))
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);
	ph->on_error = callback;
}

static size_t __csv_line(CwtCsvHandler *h)
{
	CWT_ASSERT(h);
	return ((CwtCsvHandlerImpl*)h)->line;
}


static void __csv_begin(CwtCsvHandler *h, CwtChannel *pchan)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;

	CWT_ASSERT(h);

	__csv_destroyCsvData(&ph->csvData);

	ph->csvData.ba      = __baNS->create();
	ph->csvData.tokens  = __slNS->create();
	ph->csvData.pchan   = pchan;
	ph->line            = 0;

	ph->csvData.columns = __htNS->create(__htNS->strHash, __htNS->streq, cwtFree, cwtFree);
}

static size_t __csv_header(CwtCsvHandler *h)
{
	if( __csv_next(h) ) {
		CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
		CwtStrListIterator it;
		size_t i = 0;

		__slNS->begin(ph->csvData.tokens, &it);

		while( __slNS->hasMore(&it) ) {
			CWT_CHAR *column = __strNS->dup(__slNS->next(&it));
			size_t *index = CWT_MALLOC(size_t);

			*index = i++;

			CWT_ASSERT(__htNS->insert(ph->csvData.columns, column, index));
		}
		return __slNS->size(ph->csvData.tokens);
	}
	return (size_t)0;
}


static void __csv_titles(CwtCsvHandler *h, const CWT_CHAR* argv[], size_t argc)
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

static BOOL __csv_next(CwtCsvHandler *h)
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

			if( str && __strNS->len(str) > 0 ) {

				/* not a comment */
				if( str[0] != _T('#')) {
					int rc = __slNS->split(ph->csvData.tokens, str, ph->separator, CWT_QUOTES_BOTH);

					if( rc > 0 ) {
						if( __slNS->size(ph->csvData.tokens) > ph->max_tokens ) {
							__csv_error(h, _Tr("maximum tokens in row are exceeded"));
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
			__csv_error(h, _Tr("quotes are unbalanced"));
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
static size_t __csv_columnsCount(CwtCsvHandler *h)
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
static size_t __csv_row(CwtCsvHandler *h, const CWT_CHAR* argv[], size_t argc)
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
static const CWT_CHAR* __csv_column(CwtCsvHandler *h, const CWT_CHAR* name)
{
	CwtCsvHandlerImpl *ph = (CwtCsvHandlerImpl*)h;
	size_t *index;

	CWT_ASSERT(h);

	index = __htNS->lookup(ph->csvData.columns, (void*)name);

	if( index ) {
		if( *index < __slNS->size(ph->csvData.tokens) ) {
			return __slNS->at(ph->csvData.tokens, *index);
		}
	}
	return NULL;
}
