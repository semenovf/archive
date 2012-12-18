/*
 * mysqlddi.c
 *
 *  Created on: 30.07.2012
 *      Author: user
 */


#include <cwt/dbi/dbi.h>
#include <cwt/dbi/ddi.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/logger.h>


extern CWT_CHAR* my_stringify_bool_type(void);
extern CWT_CHAR* my_stringify_int_type(LONGLONG min, ULONGLONG max);
extern CWT_CHAR* my_stringify_float_type(CwtTypeEnum time_type, UINT prec, UINT scale);
extern CWT_CHAR* my_stringify_text_type(ULONGLONG maxlen);
extern CWT_CHAR* my_stringify_blob_type(ULONGLONG maxlen);
extern CWT_CHAR* my_stringify_time_type(CwtTypeEnum time_type, BOOL stamp);

static const CWT_CHAR *__MYSQL_DEFAULT_CHARSET   = _T("utf8");
static const CWT_CHAR *__MYSQL_DEFAULT_DB_ENGINE = _T("InnoDB");



static BOOL __collect_column_definitions(CwtDDI *ddi, CwtDDIColumn *col, CwtString *tmpbuf, CWT_CHAR comma)
{
	CwtStringNS  *string_ns = cwt_string_ns();
	CwtStrListNS *sl_ns     = cwt_strlist_ns();

	CWT_CHAR *typestr = NULL;
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
	string_ns->appendChar(tmpbuf, comma);
	string_ns->append(tmpbuf, col->name);
	string_ns->appendChar(tmpbuf, _T(' '));

	if( col->ref_ptr ) {
		int n = (int)sl_ns->size(ddi->tables);

		refcol = col;

		while( col->ref_ptr && n-- ) {
			col = col->ref_ptr->pk_ptr;
		}

		if( n <= 0 ) {
			return FALSE;
		}
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

	string_ns->append(tmpbuf, typestr);

	CWT_FREE(typestr);

	if( refcol )
		col = refcol;


	if( col->flags & (UINT32)CwtDdiColumn_Nullable ) {
		string_ns->append(tmpbuf, _T(" DEFAULT NULL"));
	} else {
		if( col->default_value ) {
			string_ns->append(tmpbuf, _T(" DEFAULT "));
			string_ns->appendChar(tmpbuf, _T('\''));
			string_ns->append(tmpbuf, col->default_value);
			string_ns->appendChar(tmpbuf, _T('\''));
		} else {
			string_ns->append(tmpbuf, _T(" NOT NULL"));
		}
	}

	if( col->autoinc && !refcol )
		string_ns->append(tmpbuf, _T(" AUTO_INCREMENT"));

	return TRUE;
}

CwtStrList* my_dbd_spec_for_deploy(CwtDDI *ddi, int flags /*CwtStrList *ddiSql, const CWT_CHAR *ns, const CwtDDI *ddi, const CWT_CHAR *charset, UINT ddiflags*/)
{
	CwtStringNS  *string_ns = cwt_string_ns();
	CwtStrListNS *sl_ns     = cwt_strlist_ns();
	CwtString    tmpbuf;
	CWT_CHAR     *charset   = (CWT_CHAR*)__MYSQL_DEFAULT_CHARSET;
	CwtStrList   *spec        = NULL;
	CwtString    column_specs; /* column definitions */
	CwtString    constraint_specs;
	BOOL ok = TRUE;


	CwtListIterator tabIt;
	CwtListIterator colIt;

	spec = sl_ns->create();
	string_ns->init(&tmpbuf);
	string_ns->init(&column_specs);
	string_ns->init(&constraint_specs);

	if( ddi->charset )
		charset = ddi->charset;

	if( flags & CwtDdi_DeployDropDb ) {
		string_ns->sprintf(&tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	sl_ns->add(spec, string_ns->cstr(&tmpbuf));
	}

	string_ns->sprintf(&tmpbuf, _T("CREATE DATABASE IF NOT EXISTS `%s`"), ddi->name);
	sl_ns->add(spec, string_ns->cstr(&tmpbuf));

    string_ns->sprintf(&tmpbuf, _T("USE `%s`"), ddi->name);
    sl_ns->add(spec, string_ns->cstr(&tmpbuf));

	sl_ns->begin(ddi->tables, &tabIt);
	while( sl_ns->hasMore(&tabIt)) {
		CwtDDITable *tab = (CwtDDITable*)sl_ns->next(&tabIt);
		CWT_CHAR comma = _T(' ');

		if( flags & CwtDdi_DeployDropTab ) {
			string_ns->sprintf(&tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	sl_ns->add(spec, string_ns->cstr(&tmpbuf));
		}

		string_ns->clear(&column_specs);
		string_ns->clear(&constraint_specs);

		string_ns->sprintf(&column_specs, _T("CREATE TABLE IF NOT EXISTS `%s` ("), tab->name);

		sl_ns->begin(tab->columns, &colIt);

		while( sl_ns->hasMore(&colIt) ) {
			CwtDDIColumn *col = (CwtDDIColumn*)sl_ns->next(&colIt);
			if( !__collect_column_definitions(ddi, col, &column_specs, comma) ) {
				ok = FALSE;
				break;
			}

			comma = _T(',');

			if (col->flags & (UINT32)CwtDdiColumn_Indexable) {
				string_ns->sprintf(&tmpbuf, _T("INDEX (`%s`)"), col->name);
				if( string_ns->size(&constraint_specs) > 0 )
					string_ns->appendChar(&constraint_specs, _T(','));
				string_ns->append(&constraint_specs, string_ns->cstr(&tmpbuf));
			}

			if (col->flags & (UINT32)CwtDdiColumn_Unique) {
				string_ns->sprintf(&tmpbuf, _T("UNIQUE KEY `%s` (`%s`)"), col->name, col->name);
				if( string_ns->size(&constraint_specs) > 0 )
					string_ns->appendChar(&constraint_specs, _T(','));
				string_ns->append(&constraint_specs, string_ns->cstr(&tmpbuf));
			}

			if( col->ref_ptr ) {
				if( string_ns->size(&constraint_specs) > 0 )
					string_ns->append(&constraint_specs, _T(", "));

				string_ns->sprintf(&tmpbuf, _T("KEY `FK_%s__%s` (`%s`), ")
					, tab->name, col->name, col->name);
				string_ns->append(&constraint_specs, string_ns->cstr(&tmpbuf));

				string_ns->sprintf(&tmpbuf, _T("CONSTRAINT `FK_%s__%s` FOREIGN KEY (`%s`) REFERENCES `%s` (`%s`)")
					, tab->name, col->name, col->name, col->ref_ptr->name, col->ref_ptr->pk_ptr->name);
				string_ns->append(&constraint_specs, string_ns->cstr(&tmpbuf));
			}
		}

		if( !ok )
			break;

		if( tab->pk_ptr ) {
			string_ns->append(&column_specs, _T(", "));
			string_ns->append(&column_specs, _T("PRIMARY KEY ("));
			string_ns->append(&column_specs, tab->pk_ptr->name);
			string_ns->appendChar(&column_specs, _T(')'));
		}

		if( string_ns->size(&constraint_specs) > 0 ) {
			string_ns->append(&column_specs, _T(", "));
			string_ns->append(&column_specs, string_ns->cstr(&constraint_specs));
		}

		string_ns->appendChar(&column_specs, _T(')'));
		string_ns->append(&column_specs, _T(" ENGINE="));
		string_ns->append(&column_specs, __MYSQL_DEFAULT_DB_ENGINE);
		if( tab->autoinc_ptr ) {
			string_ns->append(&column_specs, _T(" AUTO_INCREMENT=1"));
		}

		string_ns->append(&column_specs, _T(" DEFAULT CHARSET="));
		string_ns->append(&column_specs, charset);

		sl_ns->add(spec, string_ns->cstr(&column_specs));
	}

	 string_ns->destroy(&tmpbuf);
	 string_ns->destroy(&column_specs);
	 string_ns->destroy(&constraint_specs);

	 if( !ok ) {
		 sl_ns->free(spec);
		 spec = NULL;
	 }

    return spec;
}

CwtStrList* my_dbd_spec_for_recall(CwtDDI *ddi, int flags)
{
	CwtStringNS  *string_ns = cwt_string_ns();
	CwtStrListNS *sl_ns     = cwt_strlist_ns();
	CwtString    tmpbuf     = NULL;
	CwtStrList   *spec      = NULL;

	string_ns->init(&tmpbuf);
	spec = sl_ns->create();

	if( flags & CwtDdi_RecallDropDb ) {
		string_ns->sprintf(&tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	sl_ns->add(spec, string_ns->cstr(&tmpbuf));
	} else {
		CwtListIterator tabIt;
		CwtListIterator colIt;

		string_ns->sprintf(&tmpbuf, _T("USE `%s`"), ddi->name);
    	sl_ns->add(spec, string_ns->cstr(&tmpbuf));


		sl_ns->begin(ddi->tables, &tabIt);
		while( sl_ns->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)sl_ns->next(&tabIt);

			sl_ns->begin(tab->columns, &colIt);

			while( sl_ns->hasMore(&colIt) ) {
				CwtDDIColumn *col = (CwtDDIColumn*)sl_ns->next(&colIt);

				if( col->ref_ptr ) {
					string_ns->sprintf(&tmpbuf, _T("ALTER TABLE `%s` DROP FOREIGN KEY `FK_%s__%s`")
						, tab->name, tab->name, col->name );
					sl_ns->add(spec, string_ns->cstr(&tmpbuf));
				}
			}
		}

		sl_ns->begin(ddi->tables, &tabIt);
		while( sl_ns->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)sl_ns->next(&tabIt);
			string_ns->sprintf(&tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	sl_ns->add(spec, string_ns->cstr(&tmpbuf));
		}

	}

	string_ns->destroy(&tmpbuf);
    return spec;
}
