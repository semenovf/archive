#include "../../include/cwt/debby/hiberlite/hiberlite.h"

namespace hiberlite {

RowScope::~RowScope ()
{
	for (size_t i = 0; i < atoms.size(); ++i)
		delete atoms[i];
}

void RowScope::addSimpleAssign (const pfs::string & name, const pfs::string & value)
{
	if (needComma)
		query << ", ";
	needComma = true;
	query << name << "=" << value;
}

RowScope * UpdateBean::curRow ()
{
	return rowStack.top().get_object();
}

void UpdateBean::startRow (const pfs::string & table, sqlid_t rowid, sqlid_t parent_id, sqlid_t index)
{
	shared_res<RowScope> rs(new RowScope);
	rowStack.push(rs);
	curRow()->query.clear();
	curRow()->query << "UPDATE " << table << " SET ";
	curRow()->id = rowid;
	curRow()->needComma = false;

	if (parent_id!=Database::NULL_ID){
		curRow()->addSimpleAssign(pfs::string(HIBERLITE_PARENTID_COLUMN), pfs::stringify(parent_id));
		curRow()->addSimpleAssign(pfs::string(HIBERLITE_ENTRY_INDEX_COLUMN), pfs::stringify(index));
	}
}

void UpdateBean::commitRow (cwt::debby::database & con, sqlid_t rowid)
{
	if (!curRow()->needComma) {
		rowStack.pop();
		return;
	}

	if (rowid != curRow()->id) {
		// FIXME
		//throw std::runtime_error("rowid mismatch");
		return;
	}

	pfs::string query(curRow()->query);

	query << " WHERE "
		<< HIBERLITE_PRIMARY_KEY_COLUMN << "=" << pfs::stringify(rowid) << ";";

	cwt::debby::statement sth = con.prepare(curRow()->query);

/*
	sqlite3_stmt* stmt_ptr=NULL;
	{
		sqlite3* db=con->getSQLite3Ptr();
		const char* foob;
		int rc=sqlite3_prepare_v2(db,curRow()->query.c_str(),-1,&stmt_ptr,&foob);
		database_error::database_assert(rc, con);
	}
	shared_stmt statement( new statement_ptr(stmt_ptr) );
*/

	for (size_t i = 0; i < curRow()->atoms.size(); ++i) {
		curRow()->atoms[i]->bindValue(sth/*statement->get_stmt()*/, static_cast<int>(i) + RowScope::FirstAtom);
	}

	// FIXME
	PFS_ASSERT(sth.exec());

	//throw std::logic_error("UpdateVisitor: commit row, but no row started");
	PFS_ASSERT(rowStack.size());

	rowStack.pop();
}

BeanUpdater::BeanUpdater() : AVisitor<UpdateBean>(&actor, SAVING)
{}

} //namespace hiberlite
