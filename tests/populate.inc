/*
 * populate.inc
 *
 *  Created on: Nov 13, 2013
 *      Author: wladt
 */

//using namespace cwt;
//using namespace cwt::io;
//using namespace cwt::debby;

class TraverseContext
{
	DbHandler & _dbh;
	Record    & _record;
	size_t      _line;  // current line
	size_t      _columnsExpected; // columns expected

public:
	TraverseContext(DbHandler & dbh, Record & record, size_t startline, size_t columnsExpected)
		: _dbh(dbh), _record(record), _line(0), _columnsExpected(columnsExpected)
	{}

	bool  (* populate_record)(const Vector<String> & columns, Record _record);
	bool  prologue (const Vector<String> & columns);
	bool  epliogue (const Vector<String> & /*columns*/) { return true; }
};

inline bool TraverseContext::prologue (const Vector<String> & columns)
{
	if (columns.size() != _columnsExpected) {
		Logger::error(_Fr("Incomplete number of columns %d (expected %d) at %d")
				% columns.size()
				% _columnsExpected
				% _line);
		return false;
	}

	return true;
}

static bool on_create_record (void * traverseContext, const Vector<String> & columns)
{
	TraverseContext * ctx = static_cast<TraverseContext *>(traverseContext);
	++ctx->_line;

	if (ctx->prologue(columns)) {
		if (ctx->populate_record) {
			if (ctx->_record.create(ctx->_dbh)) {
				return ctx->epliogue(columns);
			}
		}
	}
	return false;
}

static bool on_destroy_record (void * traverseContext, const Vector<String> & columns)
{
	TraverseContext * ctx = static_cast<TraverseContext *>(traverseContext);
	++ctx->_line;

	if (ctx->prologue(columns)) {
		if (ctx->populate_record) {
			if (ctx->_record.destroy(ctx->_dbh)) {
				return ctx->epliogue(columns);
			}
		}
	}
	return false;
}

