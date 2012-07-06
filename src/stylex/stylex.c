/*
 * stylex.c
 *
 *  Created on: 14.06.2012
 *      Author: wladt
 */


#include <cwt/stylex/stylex.h>
#include <cwt/string.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_int.h>
#include <cwt/algo/hash_str.h>
#include <cwt/algo/cmp_int.h>
#include <cwt/algo/cmp_str.h>


static HashTable *__cwt_media_table = NULL;
static HashTable *__cwt_wtype_table = NULL;
static HashTable *__cwt_class_table = NULL;
static HashTable *__cwt_id_table    = NULL;

extern void _cwt_colorx_init(void);
extern void _cwt_colorx_release(void);

/**
 * Инициализирует подсистему стилей
 *
 */
void cwt_stylex_init(void)
{
	__cwt_media_table = hash_table_new(string_hash, string_equal);
	__cwt_wtype_table = hash_table_new(string_hash, string_equal);
	__cwt_class_table = hash_table_new(string_hash, string_equal);
	__cwt_id_table    = hash_table_new(string_hash, string_equal);

	hash_table_register_free_functions(__cwt_media_table, cwtFree, NULL);
	hash_table_register_free_functions(__cwt_wtype_table, cwtFree, NULL);
	hash_table_register_free_functions(__cwt_class_table, cwtFree, NULL);
	hash_table_register_free_functions(__cwt_id_table,    cwtFree, NULL);

	_cwt_colorx_init();
}

/**
 * Деактивизирует подсистему стилей.
 * Освобождает ресурсы, задействованные при использовании подсистемы стилей.
 *
 */
void cwt_stylex_release(void)
{
	CWT_ASSERT(__cwt_media_table);
	CWT_ASSERT(__cwt_wtype_table);
	CWT_ASSERT(__cwt_class_table);
	CWT_ASSERT(__cwt_id_table);

	_cwt_colorx_release();

	hash_table_free(__cwt_media_table);
	hash_table_free(__cwt_wtype_table);
	hash_table_free(__cwt_class_table);
	hash_table_free(__cwt_id_table);
}


Cwt_StyleAttrSetPtr cwt_stylex_newStyleAttrSet(void)
{
	Cwt_StyleAttrSetPtr attrSet = NULL;
	size_t i = 0;

	attrSet = CWT_MALLOC(Cwt_StyleAttrSet);

	for( i = 0; i < CWT_State_Count; i++ ) {
		attrSet->m_states[i].first = NULL;
	}

	return attrSet;
}

/**
 * Клонирует стиль.
 *
 * @param clone Целевой стиль для клонирования.
 * @param orig Клонируемый стиль.
 */
void cwt_stylex_cloneStyle(Cwt_StylePtr clone, Cwt_StyleConstPtr orig)
{
	CWT_ASSERT(orig);
	CWT_ASSERT(clone);
	cwtMemCpy(clone, orig, sizeof(Cwt_Style));
}


static Cwt_StyleAttrSetPtr _cwt_stylex_styleAttrSet(HashTable* hashTable, const CWT_CHAR *name, BOOL create)
{
	Cwt_StyleAttrSetPtr attrSet = (Cwt_StyleAttrSetPtr)hash_table_lookup(hashTable, (CWT_CHAR*)name);
	if( NULL == attrSet && create ) {
		attrSet = cwt_stylex_newStyleAttrSet();
		CWT_ASSERT(hash_table_insert(__cwt_media_table, (CWT_CHAR*)name, attrSet));
	}
	return attrSet;
}

Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByMedia(const CWT_CHAR *name, BOOL create)
{
	return _cwt_stylex_styleAttrSet(__cwt_media_table, name, create);
}

Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByWType(const CWT_CHAR *name, BOOL create)
{
	return _cwt_stylex_styleAttrSet(__cwt_wtype_table, name, create);
}

Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetByClass(const CWT_CHAR *name, BOOL create)
{
	return _cwt_stylex_styleAttrSet(__cwt_class_table, name, create);
}

Cwt_StyleAttrSetPtr cwt_stylex_styleAttrSetById(const CWT_CHAR *name, BOOL create)
{
	return _cwt_stylex_styleAttrSet(__cwt_id_table, name, create);
}


/**
 * @brief Выполняет сборку стиля на основе каскадной таблицы стилей
 *
 * @param pstyle результирующий стиль
 * @param media тип носителя
 * @param id    идентификатор виджета
 * @param cls   класс виджета
 * @param state состояние виджета
 * @param wtype тип виджета
 *
 * @note Поддержка media только all или screen.
 */
void cwt_stylex_assembly(Cwt_StylePtr pstyle
	, const CWT_CHAR *media
	, const CWT_CHAR *wtype
	, const CWT_CHAR *cls
	, const CWT_CHAR *id
	, Cwt_StateType state)
{
	CWT_UNUSED(pstyle);
	CWT_UNUSED(media);
	CWT_UNUSED(wtype);
	CWT_UNUSED(cls);
	CWT_UNUSED(id);
	CWT_UNUSED(state);
}

