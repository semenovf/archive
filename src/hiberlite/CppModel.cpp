#include "../../include/cwt/debby/hiberlite/hiberlite.h"

namespace hiberlite {

void Model::add (Table t)
{
	// FIXME
	// throw std::logic_error("table ["+t.name+"] already exists");
	PFS_ASSERT(find(t.name) == cend());

	HIBERLITE_HL_DBG_DO( std::cout << "model add table " << t.name << std::endl; )
	insert(t.name, t);
}

Table::Table()
{
	add(Column(pfs::string(HIBERLITE_PRIMARY_KEY_COLUMN), pfs::string(HIBERLITE_PRIMARY_KEY_STORAGE_TYPE)));
}

void Table::add(Column c)
{
	HIBERLITE_HL_DBG_DO( std::cout << "table " << name << " add column " << c.name << " : " << c.storage_type << std::endl; )
	columns[c.name]=c;
}

bool Table::contains (const pfs::string & colname)
{
	return columns.find(colname) != columns.end();
}

pfs::string Scope::name_column (const pfs::string & name)
{
	if(prefix().size())
		return prefix() << "_" << name;
	else
		return name;
}

pfs::string Scope::full ()
{
	if(prefix().size())
		return table()+"_"+prefix();
	else
		return table();
}

} //namespace hiberlite
