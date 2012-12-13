/*
 * ddi.c
 *
 *  Created on: 30.07.2012
 *      Author: wladt
 */


#include <cwt/dbi/ddi.h>
#include <cwt/dbi/dbi.h>
#include <cwt/str.h>
#include <cwt/logger.h>


static CwtStrNS  *__strNS = NULL;
static CwtListNS *__listNS = NULL;


static void clear_type(CwtDDIColumn *col)
{
	CWT_ASSERT(col);

	/* clear autoinc */
	if( !(CWT_TYPE_IS_INTEGER(col->type) || CWT_TYPE_IS_FLOAT(col->type)) ) {
		col->autoinc = 0;
		if( col->owner_ptr->autoinc_ptr == col )
			col->owner_ptr->autoinc_ptr = NULL;
	}

	/* clear opts */
	cwt_bzero(&col->opts, sizeof(col->opts));

	/* clear reference */
	col->ref_ptr = NULL;

	/* clear type */
	col->type = CwtType_UNKNOWN;

}


static void cleanup_table(void *tab)
{
	CwtDDITable *t = (CwtDDITable*)tab;

	CWT_FREE(t->name);
	__listNS->free(t->columns);
	/*CWT_FREE(t);*/
}


static void cleanup_column(void *col)
{
	CwtDDIColumn *c = (CwtDDIColumn*)col;

	if( c->name )
		CWT_FREE(c->name);
	if( c->default_value )
		CWT_FREE(c->default_value);

	/*CWT_FREE(c);*/
}

CwtDDITable* ddi_new_table(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtDDITable tab;

	CWT_ASSERT(ddi);
	CWT_ASSERT(ddi->tables);

	cwt_bzero(&tab, sizeof(CwtDDITable));
	tab.name = __strNS->strDup(name);
	tab.columns = __listNS->create(sizeof(CwtDDIColumn), cleanup_column);

	__listNS->append(ddi->tables, &tab);

	return __listNS->last(ddi->tables);
}

CwtDDIColumn* ddi_new_column(CwtDDITable *tab, const CWT_CHAR *name)
{
	CwtDDIColumn col;

	CWT_ASSERT(tab);
	CWT_ASSERT(tab->columns);

	cwt_bzero(&col, sizeof(CwtDDIColumn));
	col.owner_ptr = tab;
	col.name  = __strNS->strDup(name);

	__listNS->append(tab->columns, &col);

	return __listNS->last(tab->columns);
}


CwtDDI* ddi_create_DDI(const CWT_CHAR *name, const CWT_CHAR *charset)
{
	CwtDDI *ddi = CWT_MALLOC(CwtDDI);

	if( !__strNS ) {
		__strNS  = cwt_str_ns();
		__listNS = cwt_list_ns();
	}

	cwt_bzero(ddi, sizeof(CwtDDI));
	ddi->name = __strNS->strDup(name);
	ddi->charset = __strNS->strDup(charset);
	ddi->tables = __listNS->create(sizeof(CwtDDITable), cleanup_table);
	return ddi;
}



void ddi_free_DDI(CwtDDI *ddi)
{
	CWT_ASSERT(ddi);

	if( ddi->name )
		CWT_FREE(ddi->name);

	if( ddi->charset )
		CWT_FREE(ddi->charset);

	if( ddi->tables )
		__listNS->free(ddi->tables);

	CWT_FREE(ddi);
}

CwtDDITable* ddi_find_table(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtListIterator it;

	CWT_ASSERT(ddi);

	if( ddi->tables ) {
		__listNS->begin(ddi->tables, &it);
		while( __listNS->hasMore(&it) ) {
			CwtDDITable *t = __listNS->next(&it);
			if( t && __strNS->strEq(name, t->name) ) {
				return t;
			}
		}
	}

	return NULL;
}

CwtDDIColumn* ddi_find_column(CwtDDITable *tab, const CWT_CHAR *name)
{
	CwtListIterator it;

	CWT_ASSERT(tab);

	if( tab->columns ) {
		__listNS->begin(tab->columns, &it);
		while( __listNS->hasMore(&it) ) {
			CwtDDIColumn *c = __listNS->next(&it);
			if( c && __strNS->strEq(name, c->name) ) {
				return c;
			}
		}
	}

	return NULL;
}

BOOL ddi_set_type_bool(CwtDDIColumn *col)
{
	clear_type(col);
	col->type = CwtType_BOOL;
	return TRUE;
}

BOOL ddi_set_type_char (CwtDDIColumn *col)
{
	clear_type(col);
	col->type = CwtType_CHAR;
	col->opts.int_opts.min = CWT_CHAR_MIN;
	col->opts.int_opts.max = CWT_CHAR_MAX;
	return TRUE;
}

BOOL ddi_set_type_int(CwtDDIColumn *col, LONGLONG min, ULONGLONG max)
{
	clear_type(col);
	col->type = CwtType_INT;
	col->opts.int_opts.min = min;
	col->opts.int_opts.max = max;
	return TRUE;
}

BOOL ddi_set_type_float(CwtDDIColumn *col)
{
	clear_type(col);
	col->type = CwtType_FLOAT;
	col->opts.float_opts.prec = 0;
	col->opts.float_opts.scale = 0;
	return TRUE;
}

BOOL ddi_set_type_double(CwtDDIColumn *col)
{
	clear_type(col);
	col->type = CwtType_DOUBLE;
	col->opts.float_opts.prec = 0;
	col->opts.float_opts.scale = 0;
	return TRUE;
}

BOOL ddi_set_type_decimal(CwtDDIColumn *col, UINT prec, UINT scale)
{
	clear_type(col);
	col->type = CwtType_DOUBLE;
	col->opts.float_opts.prec = prec;
	col->opts.float_opts.scale = scale;
	return TRUE;
}

BOOL ddi_set_type_text(CwtDDIColumn *col, ULONGLONG maxlen)
{
	clear_type(col);
	col->type = CwtType_TEXT;
	col->opts.text_opts.maxlen = maxlen;
	return TRUE;
}

BOOL ddi_set_type_blob(CwtDDIColumn *col, ULONGLONG maxlen)
{
	clear_type(col);
	col->type = CwtType_BLOB;
	col->opts.blob_opts.maxlen = maxlen;
	return TRUE;
}


/**
 *
 *
 * @param col
 * @param time_type
 * @param stamp if @c TRUE it is transformed into TIMESTAMP
 * @return
 */
BOOL ddi_set_type_time(CwtDDIColumn *col, CwtTypeEnum time_type, BOOL stamp)
{
	clear_type(col);

	switch(time_type) {
	case CwtType_TIME:
	case CwtType_DATE:
	case CwtType_DATETIME:
		col->type = time_type;
		col->opts.time_opts.stamp = stamp ? 1ULL : 0ULL;
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

BOOL ddi_set_type_ref(CwtDDIColumn *col, CwtDDITable *ref)
{
	clear_type(col);

	CWT_ASSERT(ref);
	CWT_ASSERT(ref->name);

	if( !ref->pk_ptr ) {
		cwt_logger_ns()->error(_T("primary key not set for referenced table '%s'"), ref->name);
		return FALSE;
	}

	col->type = ref->pk_ptr->type;
	__strNS->memcpy(&col->opts, &ref->pk_ptr->opts, sizeof(ref->pk_ptr->opts));
	col->ref_ptr = ref;

	return TRUE;
}

BOOL ddi_set_default(CwtDDIColumn *col, const CWT_CHAR *defaultValue)
{
	CWT_ASSERT(col);
	if( col->default_value )
		CWT_FREE(col->default_value);
	col->default_value = cwt_str_ns()->strDup(defaultValue);
	return TRUE;
}

BOOL ddi_set_autoinc (CwtDDIColumn *col, UINT inc)
{
	CWT_ASSERT(col);
	CWT_ASSERT(col->name);

	if( CWT_TYPE_IS_INTEGER(col->type) || CWT_TYPE_IS_FLOAT(col->type)) {
		col->autoinc = inc;
		col->owner_ptr->autoinc_ptr = col;
	}

	return TRUE;
}

BOOL ddi_set_nullable(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	if( yes )
		col->flags |= ((UINT32)CwtDdiColumn_Nullable);
	else
		col->flags &= ~((UINT32)CwtDdiColumn_Nullable);
	return TRUE;
}

BOOL ddi_set_indexable(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	if( yes )
		col->flags |= ((UINT32)CwtDdiColumn_Indexable);
	else
		col->flags &= ~((UINT32)CwtDdiColumn_Indexable);
	return TRUE;
}


BOOL ddi_set_unique(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	if( yes ) {
		col->flags |= ((UINT32)CwtDdiColumn_Unique);
	} else {
		if( col->owner_ptr->pk_ptr && col->owner_ptr->pk_ptr == col ) {
			col->owner_ptr->pk_ptr = NULL;
		}
		col->flags &= ~((UINT32)CwtDdiColumn_PK);
	}
	return TRUE;
}

BOOL ddi_set_pk(CwtDDIColumn *col)
{
	CWT_ASSERT(col);
	if( col->owner_ptr->pk_ptr && col->owner_ptr->pk_ptr != col ) {
		col->owner_ptr->pk_ptr->flags |= ~((UINT32)CwtDdiColumn_PK);
		ddi_set_unique(col->owner_ptr->pk_ptr, TRUE);
	}

	col->owner_ptr->pk_ptr = col;
	col->flags |= ((UINT32)CwtDdiColumn_PK);
	return TRUE;
}


/* Helper function for __ddi_deploy and __ddi_recall */
static BOOL exec_queries(CwtDBHandler *dbh, CwtDDI *ddi, int flags, CwtStrList* (*specFor) (CwtDDI*, int flags))
{
	CwtStrListNS *slNS = cwt_strlist_ns();

	CwtStrList *spec;
	CwtStrListIterator it;
	BOOL ok = TRUE;

	spec = specFor(ddi, flags);

	if( spec ) {
		slNS->begin(spec, &it);

		while( slNS->hasMore(&it) ) {
			if( !dbh->driver()->query(dbh, slNS->next(&it)) ) {
				ok = FALSE;
				break;
			}
		}

		slNS->free(spec);
	}
	return ok;
}


BOOL ddi_deploy(CwtDBHandler *dbh, CwtDDI *ddi, int flags)
{
	return exec_queries(dbh, ddi, flags, dbh->driver()->specForDeploy);
}

BOOL ddi_recall(CwtDBHandler *dbh, CwtDDI *ddi, int flags)
{
	return exec_queries(dbh, ddi, flags, dbh->driver()->specForRecall);
}


static BOOL __build_entity(CwtDDI *ddi, CwtPersistEntity *entity_ptr)
{
	CwtDDITable     *tab;
	CwtPersistEntry *entry_ptr;
	BOOL ok = TRUE;

	/* Already built */
	if (ddi_find_table(ddi, entity_ptr->name)) {
		return TRUE;
	}

	tab = ddi_new_table(ddi, entity_ptr->name);
	/*entity_ptr->table_ptr = tab;*/

	entry_ptr = &entity_ptr->entries[0];
	CWT_ASSERT(entry_ptr);

	while (ok && entry_ptr->name) {
		CwtDDIColumn *col;

		col = ddi_new_column(tab, entry_ptr->name);

		switch (entry_ptr->type) {
		case CwtType_UNKNOWN:
			if (entry_ptr->ref_ptr != NULL) {
				CwtPersistEntity *entity_ref = (CwtPersistEntity *)entry_ptr->ref_ptr;
				CwtDDITable      *table_ref;

				table_ref = ddi_find_table(ddi, entity_ref->name);

				if( table_ref ) {
					ok = ddi_set_type_ref(col, table_ref);
				} else {
					/* TODO necessary to implement the means to detect and prevent an infinite loop */
					ok = __build_entity(ddi, entity_ref);
				}
			} else {
				ok = FALSE;
			}
			break;
		case CwtType_BOOL:
			ok = ddi_set_type_bool(col);
			break;
		case CwtType_CHAR:
			ok = ddi_set_type_char(col);
			break;
		case CwtType_SBYTE:
		case CwtType_BYTE:
		case CwtType_SHORT:
		case CwtType_USHORT:
		case CwtType_INT:
		case CwtType_UINT:
		case CwtType_LONG:
		case CwtType_ULONG:
		case CwtType_LONGLONG:
		case CwtType_ULONGLONG:
			ok = ddi_set_type_int(col
					, entry_ptr->opts.int_opts.min
					, entry_ptr->opts.int_opts.max);
			break;

		case CwtType_FLOAT:
		case CwtType_DOUBLE:
			if ( entry_ptr->opts.float_opts.prec != 0.0
					&& entry_ptr->opts.float_opts.scale != 0 ) {
				ok = ddi_set_type_decimal(col
					, (UINT)entry_ptr->opts.float_opts.prec
					, (UINT)entry_ptr->opts.float_opts.scale);
			} else {
				if (CwtType_FLOAT == entry_ptr->type )
					ok = ddi_set_type_float(col);
				else
					ok = ddi_set_type_double(col);
			}
			break;
		case CwtType_TEXT:
			ok = ddi_set_type_text(col, entry_ptr->opts.text_opts.maxlen);
			break;
		case CwtType_BLOB:
			ok = ddi_set_type_text(col, entry_ptr->opts.blob_opts.maxlen);
			break;
		case CwtType_TIME:
		case CwtType_DATE:
		case CwtType_DATETIME:
			ok = ddi_set_type_time(col, entry_ptr->type, entry_ptr->opts.time_opts.stamp ? TRUE : FALSE);
			break;
		default:
			break;
		}

		ok = ddi_set_nullable(col
				, (entry_ptr->flags & (UINT32)CwtDdiColumn_Nullable)
					? TRUE : FALSE );

		ok = ddi_set_unique(col
				, (entry_ptr->flags & (UINT32)CwtDdiColumn_Unique)
					? TRUE : FALSE );

		ok = ddi_set_indexable(col
				, (entry_ptr->flags & (UINT32)CwtDdiColumn_Indexable)
					? TRUE : FALSE );

		if (entry_ptr->autoinc > 0)
			ddi_set_autoinc(col, entry_ptr->autoinc);

		if (entry_ptr->flags & (UINT32)CwtDdiColumn_PK)
			ok = ddi_set_pk(col);

		entry_ptr++;
	}

	return ok;
}

CwtDDI* ddi_build_DDI (const CWT_CHAR *name, const CWT_CHAR *charset, CwtPersistEntity *entities[], size_t nentries)
{
	CwtDDI *ddi;
	BOOL ok = TRUE;
	size_t i = 0;

	ddi = ddi_create_DDI(name, charset);

	for (i = 0; i < nentries && ok; i++) {
		ok = __build_entity(ddi, entities[i]);
	}

	if (!ok) {
		ddi_free_DDI(ddi);
		ddi = NULL;
	}

	return ddi;
}
