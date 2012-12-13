/**
 * @file   sqlite3_ddi.c
 * @author wladt
 * @date   Nov 28, 2012 10:00:17 AM
 *
 * @brief
 */


#include "cwt/dbi/dbi.h"
#include "cwt/dbi/ddi.h"
/*
#include "cwt/str.h"
#include "cwt/string.h"
#include "cwt/strlist.h"
#include "cwt/logger.h"
*/


extern CWT_CHAR* sqlite3_stringify_bool_type(void);
extern CWT_CHAR* sqlite3_stringify_int_type(LONGLONG min, ULONGLONG max);
extern CWT_CHAR* sqlite3_stringify_float_type(CwtTypeEnum time_type, UINT prec, UINT scale);
extern CWT_CHAR* sqlite3_stringify_text_type(ULONGLONG maxlen);
extern CWT_CHAR* sqlite3_stringify_blob_type(ULONGLONG maxlen);
extern CWT_CHAR* sqlite3_stringify_time_type(CwtTypeEnum time_type, BOOL stamp);

static const CWT_CHAR *__SQLITE_DEFAULT_CHARSET = _T("UTF-8");
/*
static const CWT_CHAR *__MYSQL_DEFAULT_DB_ENGINE = _T("InnoDB");
*/


static BOOL __collect_column_definitions(CwtDDI *ddi, CwtDDIColumn *col, CwtString *tmpbuf, CWT_CHAR comma)
{
	CwtStringNS  *stringNS = cwt_string_ns();
	CwtStrListNS *slNS     = cwt_strlist_ns();

	CWT_CHAR *typestr = NULL;
	/*BOOL is_ref       = FALSE;*/
	CwtDDIColumn *refcol = NULL;

/*
	col_name data_type
		[NOT NULL | NULL]
		[DEFAULT default_value] - TODO not supported yet
	    [AUTO_INCREMENT]
	    [UNIQUE [KEY] | [PRIMARY] KEY]
	    [COMMENT 'string']                      - not supported yet
	    [COLUMN_FORMAT {FIXED|DYNAMIC|DEFAULT}] - not supported yet
*/
	/*stringNS->clear(tmpbuf);*/
	stringNS->appendChar(tmpbuf, comma);
	stringNS->append(tmpbuf, col->name);
	stringNS->appendChar(tmpbuf, _T(' '));

	if( col->ref_ptr ) {
		int n = (int)slNS->size(ddi->tables);

		refcol = col;

		while( col->ref_ptr && n-- ) {
			col = col->ref_ptr->pk_ptr;
		}

		if( n <= 0 ) {
			return FALSE;
		}

		/*is_ref = TRUE;*/
	}

	if( col->type != CwtType_UNKNOWN ) {
		if( col->type == CwtType_BOOL ) {
			typestr = my_stringify_bool_type();
		} else if( CWT_TYPE_IS_INTEGER(col->type) ) {
			typestr = my_stringify_int_type(col->opts.int_opts.min, col->opts.int_opts.max);
		} else if(CWT_TYPE_IS_FLOAT(col->type)) {
			typestr = my_stringify_float_type(col->type
					, (UINT)col->opts.float_opts.prec
					, (UINT)col->opts.float_opts.scale);
		} else if( CwtType_TEXT == col->type ) {
			typestr = my_stringify_text_type(col->opts.text_opts.maxlen);
		} else if( CwtType_BLOB == col->type ) {
			typestr = my_stringify_blob_type(col->opts.blob_opts.maxlen);
		} else if( CwtType_TIME == col->type
				|| CwtType_DATE == col->type
				|| CwtType_DATETIME == col->type ) {
			typestr = my_stringify_time_type(col->type, col->opts.time_opts.stamp ? TRUE : FALSE);
		} else {
			CWT_ASSERT(FALSE);
		}
	}

	stringNS->append(tmpbuf, typestr);

	CWT_FREE(typestr);

	if( refcol )
		col = refcol;


	if( col->flags & (UINT32)CwtDdiColumn_Nullable ) {
		stringNS->append(tmpbuf, _T(" DEFAULT NULL"));
	} else {
		if( col->default_value ) {
			stringNS->append(tmpbuf, _T(" DEFAULT "));
			stringNS->appendChar(tmpbuf, _T('\''));
			stringNS->append(tmpbuf, col->default_value);
			stringNS->appendChar(tmpbuf, _T('\''));
		} else {
			stringNS->append(tmpbuf, _T(" NOT NULL"));
		}
	}

	if( col->autoinc && !refcol )
		stringNS->append(tmpbuf, _T(" AUTO_INCREMENT"));

	return TRUE;
}

CwtStrList* sqlite3_dbd_spec_for_deploy(CwtDDI *ddi, int flags)
{
	CwtStrNS     *str_ns   = cwt_str_ns();
	CwtStringNS  *stringNS = cwt_string_ns();
	CwtStrListNS *slNS     = cwt_strlist_ns();
	CwtString    *tmpbuf   = stringNS->create();
	CWT_CHAR     *charset  = (CWT_CHAR*)__SQLITE_DEFAULT_CHARSET;

	CwtStrList   *spec         = NULL;
	CwtString    *columnSpecs  = NULL; /* column definitions */
/*	CwtString    *indexSpecs   = NULL;*/
	CwtString    *constraintSpecs = NULL;
	BOOL ok = TRUE;


	CwtListIterator tabIt;
	CwtListIterator colIt;

	tmpbuf = stringNS->create();
	spec = slNS->create();
	columnSpecs = stringNS->create();
	/*indexSpecs = stringNS->create();*/
	constraintSpecs = stringNS->create();

	if( ddi->charset )
		charset = ddi->charset;

	if(str_ns->strCaseEq(charset, _T("UTF-16")))
		stringNS->sprintf(tmpbuf, _T("PRAGMA encoding =\"UTF-16\";"));
	else if(str_ns->strCaseEq(charset, _T("UTF-16le")))
		stringNS->sprintf(tmpbuf, _T("PRAGMA encoding =\"UTF-16le\";"));
	else if(str_ns->strCaseEq(charset, _T("UTF-16be")))
		stringNS->sprintf(tmpbuf, _T("PRAGMA encoding =\"UTF-16be\";"));
	if(str_ns->strCaseEq(charset, _T("UTF-8")))
		stringNS->sprintf(tmpbuf, _T("PRAGMA encoding =\"UTF-8\";"));
	else {
		cwt_logger_ns()->warn(_Tr("%s: unsupported encoding, setting to UTF-8"), charset);
		stringNS->sprintf(tmpbuf, _T("PRAGMA encoding =\"UTF-8\";"));
	}

	if( flags & CwtDdi_DeployDropDb ) {
		stringNS->sprintf(tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf));
	}

	stringNS->sprintf(tmpbuf, _T("CREATE DATABASE IF NOT EXISTS `%s`"), ddi->name);
	slNS->add(spec, stringNS->cstr(tmpbuf));

    stringNS->sprintf(tmpbuf, _T("USE `%s`"), ddi->name);
    slNS->add(spec, stringNS->cstr(tmpbuf));

	slNS->begin(ddi->tables, &tabIt);
	while( slNS->hasMore(&tabIt)) {
		CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);
		CWT_CHAR comma = _T(' ');

		if( flags & CwtDdi_DeployDropTab ) {
			stringNS->sprintf(tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	slNS->add(spec, stringNS->cstr(tmpbuf));
		}

		/*stringNS->clear(indexSpecs);*/
		stringNS->clear(columnSpecs);
		stringNS->clear(constraintSpecs);

		stringNS->sprintf(columnSpecs, _T("CREATE TABLE IF NOT EXISTS `%s` ("), tab->name);

		slNS->begin(tab->columns, &colIt);

		while( slNS->hasMore(&colIt) ) {
			CwtDDIColumn *col = (CwtDDIColumn*)slNS->next(&colIt);
			if( !__collect_column_definitions(ddi, col, columnSpecs, comma) ) {
				ok = FALSE;
				break;
			}

			comma = _T(',');

			if (col->flags & (UINT32)CwtDdiColumn_Indexable) {
				stringNS->sprintf(tmpbuf, _T("INDEX (`%s`)"), col->name);
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->appendChar(constraintSpecs, _T(','));
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}

			if (col->flags & (UINT32)CwtDdiColumn_Unique) {
				stringNS->sprintf(tmpbuf, _T("UNIQUE KEY `%s` (`%s`)"), col->name, col->name);
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->appendChar(constraintSpecs, _T(','));
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}

			if( col->ref_ptr ) {
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->append(constraintSpecs, _T(", "));

				stringNS->sprintf(tmpbuf, _T("KEY `FK_%s__%s` (`%s`), ")
					, tab->name, col->name, col->name);
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));

				stringNS->sprintf(tmpbuf, _T("CONSTRAINT `FK_%s__%s` FOREIGN KEY (`%s`) REFERENCES `%s` (`%s`)")
					, tab->name, col->name, col->name, col->ref_ptr->name, col->ref_ptr->pk_ptr->name);
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}
		}

		if( !ok )
			break;

		if( tab->pk_ptr ) {
			stringNS->append(columnSpecs, _T(", "));
			stringNS->append(columnSpecs, _T("PRIMARY KEY ("));
			stringNS->append(columnSpecs, tab->pk_ptr->name);
			stringNS->appendChar(columnSpecs, _T(')'));
		}

/*
		if( stringNS->size(indexSpecs) > 0 ) {
			stringNS->append(columnSpecs, _T(", "));
			stringNS->append(columnSpecs, stringNS->cstr(indexSpecs));
		}
*/

		if( stringNS->size(constraintSpecs) > 0 ) {
			stringNS->append(columnSpecs, _T(", "));
			stringNS->append(columnSpecs, stringNS->cstr(constraintSpecs));
		}

		stringNS->appendChar(columnSpecs, _T(')'));
		stringNS->append(columnSpecs, _T(" ENGINE="));
		stringNS->append(columnSpecs, __MYSQL_DEFAULT_DB_ENGINE);
		if( tab->autoinc_ptr ) {
			stringNS->sprintf(tmpbuf, _T(" AUTO_INCREMENT=%d"), tab->autoinc_ptr->autoinc);
			stringNS->append(columnSpecs, stringNS->cstr(tmpbuf));
		}

		stringNS->append(columnSpecs, _T(" DEFAULT CHARSET="));
		stringNS->append(columnSpecs, charset);

		slNS->add(spec, stringNS->cstr(columnSpecs));
	}

	 stringNS->free(tmpbuf);
	 stringNS->free(columnSpecs);
	 /*stringNS->free(indexSpecs);*/
	 stringNS->free(constraintSpecs);

	 if( !ok ) {
		 slNS->free(spec);
		 spec = NULL;
	 }

    return spec;
}

CwtStrList* sqlite3_dbd_spec_for_recall(CwtDDI *ddi, int flags)
{
	CwtStringNS  *stringNS = cwt_string_ns();
	CwtStrListNS *slNS     = cwt_strlist_ns();
	CwtString    *tmpbuf   = stringNS->create();
	CwtStrList   *spec     = NULL;

	tmpbuf = stringNS->create();
	spec = slNS->create();

	if( flags & CwtDdi_RecallDropDb ) {
		stringNS->sprintf(tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf));
	} else {
		CwtListIterator tabIt;
		CwtListIterator colIt;

		stringNS->sprintf(tmpbuf, _T("USE `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf));


		slNS->begin(ddi->tables, &tabIt);
		while( slNS->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);

			slNS->begin(tab->columns, &colIt);

			while( slNS->hasMore(&colIt) ) {
				CwtDDIColumn *col = (CwtDDIColumn*)slNS->next(&colIt);

				if( col->ref_ptr ) {
					stringNS->sprintf(tmpbuf, _T("ALTER TABLE `%s` DROP FOREIGN KEY `FK_%s__%s`")
						, tab->name, tab->name, col->name );
					slNS->add(spec, stringNS->cstr(tmpbuf));
				}
			}
		}

		slNS->begin(ddi->tables, &tabIt);
		while( slNS->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);
			stringNS->sprintf(tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	slNS->add(spec, stringNS->cstr(tmpbuf));
		}

	}

    return spec;
}
