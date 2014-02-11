/**
 * @file schema.hpp

 * @author wladt
 * @date Feb 10, 2014
 *
 * @note Based on [hiberlite](https://code.google.com/p/hiberlite/) library
 */

#ifndef __CWT_DEBBY_SCHEMA_HPP__
#define __CWT_DEBBY_SCHEMA_HPP__

#include <cwt/debby/hiberlite/hiberlite.h>

namespace cwt { namespace debby {

#ifdef __COMMENT__
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
};

#endif

}} // cwt::debby

#endif /* __CWT_DEBBY_SCHEMA_HPP__ */
