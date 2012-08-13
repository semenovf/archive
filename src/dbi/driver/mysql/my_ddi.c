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


extern CWT_CHAR* __stringifyBoolType(void);
extern CWT_CHAR* __stringifyIntType(LONGLONG min, ULONGLONG max);
extern CWT_CHAR* __stringifyFloatType(UINT prec, UINT scale);
extern CWT_CHAR* __stringifyTextType(ULONGLONG maxlen);
extern CWT_CHAR* __stringifyBlobType(ULONGLONG maxlen);
extern CWT_CHAR* __stringifyTimeType(CwtTypeEnum time_type, BOOL stamp);

static const CWT_CHAR *__MYSQL_DEFAULT_CHARSET   = _T("utf8");
static const CWT_CHAR *__MYSQL_DEFAULT_DB_ENGINE = _T("InnoDB");



static BOOL __collect_column_definitions(CwtDDI *ddi, CwtDDIColumn *col, CwtString *tmpbuf, CWT_CHAR comma)
{
	CwtStringNS  *stringNS = cwtStringNS();
	CwtStrListNS *slNS     = cwtStrListNS();

	CWT_CHAR *typestr = NULL;
	BOOL is_ref       = FALSE;

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

	if( col->pRef ) {
		int n = (int)slNS->size(ddi->tables);
		while( col->pRef && n-- ) {
			col = col->pRef->pPK;
		}

		if( n <= 0 ) {
			return FALSE;
		}

		is_ref = TRUE;
	}

	if( col->type != CwtType_UNKNOWN ) {
		if( col->type == CwtType_BOOL ) {
			typestr = __stringifyBoolType();
		} else if( CWT_TYPE_IS_INTEGER(col->type) ) {
			typestr = __stringifyIntType(col->opts.int_opts.min, col->opts.int_opts.max);
		} else if(CWT_TYPE_IS_FLOAT(col->type)) {
			typestr = __stringifyFloatType(col->opts.float_opts.prec, col->opts.float_opts.scale);
		} else if( CwtType_TEXT == col->type ) {
			typestr = __stringifyTextType(col->opts.maxlen);
		} else if( CwtType_BLOB == col->type ) {
			typestr = __stringifyBlobType(col->opts.maxlen);
		} else if( CwtType_TIME == col->type
				|| CwtType_DATE == col->type
				|| CwtType_DATETIME == col->type ) {
			typestr = __stringifyTimeType(col->type, col->opts.stamp);
		} else {
			CWT_ASSERT(FALSE);
		}
	}

	stringNS->append(tmpbuf, typestr);

	CWT_FREE(typestr);

	if( col->is_null ) {
		stringNS->append(tmpbuf, _T(" DEFAULT NULL"));
	} else {
		if( col->defaultValue ) {
			stringNS->append(tmpbuf, _T(" DEFAULT "));
			stringNS->appendChar(tmpbuf, _T('\''));
			stringNS->append(tmpbuf, col->defaultValue);
			stringNS->appendChar(tmpbuf, _T('\''));
		} else {
			stringNS->append(tmpbuf, _T(" NOT NULL"));
		}
	}

	if( col->autoinc && !is_ref )
		stringNS->append(tmpbuf, _T(" AUTO_INCREMENT"));

	return TRUE;
}

CwtStrList* __specForDeploy(CwtDDI *ddi, int flags /*CwtStrList *ddiSql, const CWT_CHAR *ns, const CwtDDI *ddi, const CWT_CHAR *charset, UINT ddiflags*/)
{
	CwtStrNS     *strNS    = cwtStrNS();
	CwtStringNS  *stringNS = cwtStringNS();
	CwtStrListNS *slNS     = cwtStrListNS();
	CwtString    *tmpbuf   = stringNS->create();
	BOOL          ok       = TRUE;
	CWT_CHAR     *charset  = (CWT_CHAR*)__MYSQL_DEFAULT_CHARSET;

	CwtStrList   *spec         = NULL;
	CwtString    *columnSpecs  = NULL; /* column definitions */
/*	CwtString    *indexSpecs   = NULL;*/
	CwtString    *constraintSpecs = NULL;


	CwtListIterator tabIt;
	CwtListIterator colIt;

	tmpbuf = stringNS->create();
	spec = slNS->create();
	columnSpecs = stringNS->create();
	/*indexSpecs = stringNS->create();*/
	constraintSpecs = stringNS->create();

	if( ddi->charset )
		charset = ddi->charset;

	if( flags & CWT_DDI_DEPLOY_DROP_DB ) {
		stringNS->sprintf(tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));
	}

	stringNS->sprintf(tmpbuf, _T("CREATE DATABASE IF NOT EXISTS `%s`"), ddi->name);
	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));

    stringNS->sprintf(tmpbuf, _T("USE `%s`"), ddi->name);
    slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));

	slNS->begin(ddi->tables, &tabIt);
	while( slNS->hasMore(&tabIt)) {
		CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);
		CWT_CHAR comma = _T(' ');

		if( flags & CWT_DDI_DEPLOY_DROP_TAB ) {
			stringNS->sprintf(tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));
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

			if( col->is_index ) {
				stringNS->sprintf(tmpbuf, _T("INDEX (`%s`)"), col->name);
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->appendChar(constraintSpecs, _T(','));
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}

			if( col->is_uniq ) {
				stringNS->sprintf(tmpbuf, _T("UNIQUE KEY `%s` (`%s`)"), col->name, col->name);
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->appendChar(constraintSpecs, _T(','));
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}

			if( col->pRef ) {
				if( stringNS->size(constraintSpecs) > 0 )
					stringNS->append(constraintSpecs, _T(", "));

				stringNS->sprintf(tmpbuf, _T("KEY `FK_%s__%s` (`%s`), ")
					, tab->name, col->name, col->name);
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));

				stringNS->sprintf(tmpbuf, _T("CONSTRAINT `FK_%s__%s` FOREIGN KEY (`%s`) REFERENCES `%s` (`%s`)")
					, tab->name, col->name, col->name, col->pRef->name, col->pRef->pPK->name);
				stringNS->append(constraintSpecs, stringNS->cstr(tmpbuf));
			}
		}

		if( !ok )
			break;

		if( tab->pPK ) {
			stringNS->append(columnSpecs, _T(", "));
			stringNS->append(columnSpecs, _T("PRIMARY KEY ("));
			stringNS->append(columnSpecs, tab->pPK->name);
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
		if( tab->pAutoinc ) {
			stringNS->sprintf(tmpbuf, _T(" AUTO_INCREMENT=%d"), tab->pAutoinc->autoinc);
			stringNS->append(columnSpecs, stringNS->cstr(tmpbuf));
		}

		stringNS->append(columnSpecs, _T(" DEFAULT CHARSET="));
		stringNS->append(columnSpecs, charset);

		slNS->add(spec, stringNS->cstr(columnSpecs), stringNS->size(columnSpecs));
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

CwtStrList* __specForRecall(CwtDDI *ddi, int flags)
{
	CwtStrNS     *strNS    = cwtStrNS();
	CwtStringNS  *stringNS = cwtStringNS();
	CwtStrListNS *slNS     = cwtStrListNS();
	CwtString    *tmpbuf   = stringNS->create();
	BOOL          ok       = TRUE;

	CwtStrList   *spec         = NULL;

	tmpbuf = stringNS->create();
	spec = slNS->create();

	if( flags & CWT_DDI_RECALL_DROP_DB ) {
		stringNS->sprintf(tmpbuf, _T("DROP DATABASE IF EXISTS `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));
	} else {
		CwtListIterator tabIt;
		CwtListIterator colIt;

		stringNS->sprintf(tmpbuf, _T("USE `%s`"), ddi->name);
    	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));


		slNS->begin(ddi->tables, &tabIt);
		while( slNS->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);

			slNS->begin(tab->columns, &colIt);

			while( slNS->hasMore(&colIt) ) {
				CwtDDIColumn *col = (CwtDDIColumn*)slNS->next(&colIt);

				if( col->pRef ) {
					stringNS->sprintf(tmpbuf, _T("ALTER TABLE `%s` DROP FOREIGN KEY `FK_%s__%s`")
						, tab->name, tab->name, col->name );
					slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));
				}
			}
		}

		slNS->begin(ddi->tables, &tabIt);
		while( slNS->hasMore(&tabIt)) {
			CwtDDITable *tab = (CwtDDITable*)slNS->next(&tabIt);
			stringNS->sprintf(tmpbuf, _T("DROP TABLE IF EXISTS `%s`"), tab->name);
	    	slNS->add(spec, stringNS->cstr(tmpbuf), stringNS->size(tmpbuf));
		}

	}

    return spec;
}
