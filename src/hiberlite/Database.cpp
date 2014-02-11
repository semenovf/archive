#include "../../include/cwt/debby/hiberlite/hiberlite.h"

namespace hiberlite {

Database::Database () : mx(nullptr)
{
}

Database::Database (const pfs::string & dburi) : mx(nullptr)
{
	open(dburi);
}

Database::~Database ()
{
	close();
}

void Database::open (const pfs::string & dburi)
{
	con.open(dburi);
}

void Database::close ()
{
	if (mx) {
		delete mx;
		mx = nullptr;
	}
}

std::vector<pfs::string> Database::checkModel ()
{
	//TODO checkModel
	std::vector<pfs::string> ans;
	return ans;
}

void Database::dropModel ()
{
	if (mx) {
		Model mdl = mx->getModel();
		for (Model::iterator it = mdl.begin(); it != mdl.end(); ++it) {
			Table & t = it->second;
			pfs::string q;
			q << "DROP TABLE IF EXISTS " << t.name << ";";
			dbExecQuery(q);
		}
	}
}

void Database::createModel ()
{
	if (mx) {
		Model mdl = mx->getModel();

		for (Model::iterator it = mdl.begin(); it != mdl.end(); ++it) {
			Table & t = it->second;
			pfs::string query;
			query << "CREATE TABLE " << t.name << " (";
			bool needComma = false;

			for (std::map<pfs::string, Column>::iterator c = t.columns.begin(); c != t.columns.end(); ++c) {
				if(needComma)
					query << ", ";
				needComma = true;

				Column & col = c->second;
				query << col.name << " ";

				if (col.name == HIBERLITE_PRIMARY_KEY_COLUMN)
					query << "INTEGER PRIMARY KEY AUTOINCREMENT";
				else
					query << col.storage_type;
			}
			query << ");";
			dbExecQuery(query);
		}
	}
}

sqlid_t Database::allocId (cwt::debby::database & dbh, const pfs::string & table)
{
	//THREAD critical call
	//char * err_msg = NULL;
	pfs::string query;
	query << "INSERT INTO " << table << " (" << HIBERLITE_PRIMARY_KEY_COLUMN << ") VALUES (NULL);";
	HIBERLITE_HL_DBG_DO( std::cout << "exec: " << query << std::endl; )

	cwt::debby::statement sth = dbh.prepare(query);

// FIXME
//	if(err_msg)
//		throw database_error(err_msg);
//	database_error::database_assert(rc, c);
	PFS_ASSERT(sth.exec());

	return sth.lastId();
}

void Database::dbExecQuery (cwt::debby::database con, const pfs::string & query)
{
	HIBERLITE_HL_DBG_DO( std::cout << "exec: " << query << std::endl; )

	cwt::debby::statement sth = con.prepare(query);
	sth.exec();
}

sqlid_t Database::allocId (const pfs::string & table)
{
	return allocId(con, table);
}

void Database::dbExecQuery (const pfs::string & query)
{
	return dbExecQuery(con, query);
}

std::vector<sqlid_t> Database::dbSelectIds (cwt::debby::database & con
		, const pfs::string & table
		, const pfs::string & condition
		, const pfs::string & orderBy)
{
	pfs::string where;

	if(condition.size())
		where << " WHERE " << condition;

	pfs::string order;

	if (orderBy.size())
		order << " ORDER BY " << orderBy;

	pfs::string query;
	query << "SELECT " << pfs::string(HIBERLITE_PRIMARY_KEY_COLUMN)
			<< " FROM " << table
			<< where
			<< order
			<< ";";

	cwt::debby::statement sth = con.prepare(query);
	std::vector<sqlid_t> ans;

	if (sth.exec()) {
		pfs::vector<pfs::unitype> row;

		while (!(row = sth.fetchRowArray()).isEmpty())
			ans.push_back(row[0].toLong());
	}
	return ans;
}

std::vector<sqlid_t> Database::dbSelectChildIds(cwt::debby::database & con, const pfs::string & table, sqlid_t parent)
{
	return dbSelectIds(con, table, pfs::string(HIBERLITE_PARENTID_COLUMN) << "=" << pfs::stringify(parent)
		, pfs::string(HIBERLITE_ENTRY_INDEX_COLUMN));
}

std::vector<sqlid_t> Database::dbSelectChildIds(const pfs::string & table, sqlid_t parent)
{
	return dbSelectChildIds(con, table, parent);
}


} //namespace hiberlite;
