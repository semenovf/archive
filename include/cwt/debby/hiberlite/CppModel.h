#ifndef __HIBERLITE_CPPMODEL_H__
#define __HIBERLITE_CPPMODEL_H__

namespace hiberlite {

class Scope
{
public:
	pfs::string _table, _prefix;
	int prefix_depth;

	inline Scope (const pfs::string & t, const pfs::string & p)
		: _table(t)
		, _prefix(p)
		, prefix_depth(0) {}
	inline Scope () : prefix_depth(0) {}

	inline pfs::string table () { return _table; }
	inline pfs::string prefix () { return _prefix; }

	pfs::string name_column (const pfs::string & name);
	pfs::string full ();
};

class Column
{
public:
	inline Column(const pfs::string & _n = pfs::string(), const pfs::string & _s = pfs::string())
		: name(_n), storage_type(_s) {}
	pfs::string name;
	pfs::string storage_type;
};

class Table
{
public:
	Table();
	pfs::string name;
	std::map<pfs::string,Column> columns;
	void add (Column c);
	bool contains (const pfs::string & colname);
};

class Model : public pfs::map<pfs::string, Table> {
public:
	void add (Table t);
};

} //namespace hiberlite

#endif // __HIBERLITE_CPPMODEL_H__
