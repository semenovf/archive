/**
 * @file schema.hpp

 * @author wladt
 * @date Feb 10, 2014
 *
 * @note Based on [hiberlite](https://code.google.com/p/hiberlite/) library
 */

#ifndef __CWT_DEBBY_SCHEMA_HPP__
#define __CWT_DEBBY_SCHEMA_HPP__

//#include <cwt/debby/hiberlite/hiberdefs.h>
//#include <cwt/debby/hiberlite/ModelExtractor.h>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <cwt/errorable.hpp>

#define HIBERLITE_NVP(_Field) hiberlite::sql_nvp< decltype(_Field) >(#_Field, _Field)

//#define HIBERLITE_BASE_CLASS(ClName) hiberlite::sql_nvp< ClName >(#ClName,*((ClName*)this) )

//#define HIBERLITE_PRIMARY_KEY_COLUMN "hiberlite_id"
//#define HIBERLITE_PARENTID_COLUMN "hiberlite_parent_id"
//#define HIBERLITE_ENTRY_INDEX_COLUMN "hiberlite_entry_indx"
//#define HIBERLITE_ID_STORAGE_CLASS "INTEGER"
//#define HIBERLITE_PRIMARY_KEY_STORAGE_TYPE "PRIMARYKEY"

#define CWT_DEBBY_EXPORT_CLASS(_Class) 		    \
	namespace cwt { namespace debby {			\
	template<>									\
	pfs::string schema::getClassName<_Class>()	\
	{ return #_Class; }}}

namespace cwt { namespace debby {

class column
{
	pfs::string _name;
	pfs::string _storageType;

public:
	column (const pfs::string & name = pfs::string()
			, const pfs::string & storageType = pfs::string())
		: _name(name)
		, _storageType(storageType) {}

	pfs::string name        () const { return _name; }
	pfs::string storageType () const { return _storageType; }
};

class table
{
	pfs::string _name;
	pfs::map<pfs::string, column> _columns;

public:
	table ();

	pfs::string name () const { return _name; }
	void add (const column & col);
	bool contains (const pfs::string & colname) const;
};


class schema : public pfs::map<pfs::string, table>, public errorable
{
	//hiberlite::ModelExtractor * mx;
public:
	static const pfs::string PrimaryKeyColumn;
	static const pfs::string PrimaryKeyStorageType;

public:
	virtual ~schema () {}

	void add (table t);
	void drop ();
	void create ();

	template<class C>
	static std::string getClassName ();

	template<class _BeanClass>
	void registerBeanClass ();
};

template<class _BeanClass>
inline void schema::registerBeanClass ()
{
/*
	if (!mx)
		mx = new hiberlite::ModelExtractor();
	std::string classname = schema::getClassName<_BeanClass>();

	mx->extractModel<_BeanClass>(con);
*/
}


}} // cwt::debby

#endif /* __CWT_DEBBY_SCHEMA_HPP__ */
