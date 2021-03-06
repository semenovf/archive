/*
 * model.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/vector.hpp>
#include <cwt/fs.hpp>
#include <cwt/debby/schema.hpp>

// XXX Insertion in Sqlite3 is very slow (one transaction for each insertion), so it is good idea
//     to wrap series of insertions with transaction.


struct A
{
	char ch;
	short sh;
	int i;
};

struct B
{
	pfs::string text;
	pfs::bytearray blob;
};

struct C
{
	A a;
	pfs::vector<B> b;
};


class abstract_bean
{
public:
	virtual bool create () = 0;
	virtual bool read () = 0;
	virtual bool update () = 0;
	virtual bool destroy () = 0;
};

template <typename _Class>
class bean : public abstract_bean
{
public:
	virtual ~bean () {}
	virtual bool create () override;
	virtual bool read () override;
	virtual bool update () override;
	virtual bool destroy () override;
};

void open_schema (cwt::debby::database & db)
{
	// sqlite3:///tmp/test.db?mode=rwc
	pfs::string uri;
	cwt::fs fs;
	uri << "sqlite3://"
		<< fs.tempDirectory()
		<< "/test_schema.sqlite3";

	db.open(uri);

	TEST_FAIL_X(db.opened(), db.logErrors());
}

void drop_schema (cwt::debby::database & db)
{
	db.begin();
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS a")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS b")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS c")), db.logErrors());
	db.commit();
}

void create_schema (cwt::debby::database & db)
{
	pfs::string sql_a, sql_b, sql_c;

	db.begin();
	// drop schema
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS a")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS b")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS c")), db.logErrors());

	pfs::string pk("id INTEGER PRIMARY KEY NOT NULL"); // mandatory implicit field
	pfs::string table_name;

	table_name = pfs::string("a");
	sql_a << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", ch INTEGER NOT NULL"
			<< ", sh INTEGER NOT NULL"
			<< ", i  INTEGER NOT NULL"
			<< ")";

	table_name = pfs::string("b");
	sql_b << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", text TEXT NOT NULL"
			<< ", blob BLOB NOT NULL"
//			<< ", c_id INTEGER NOT NULL"
//			<< ", CONSTRAINT c_fk FOREIGN KEY (c_id) REFERENCES c (id)"
			<< ");";

	table_name = pfs::string("c");
	sql_c << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", a_id INTEGER NOT NULL"
			<< ", CONSTRAINT a_fk FOREIGN KEY (a_id) REFERENCES a (id)"
			<< ");";

	// Represented vector field in class C
	pfs::string sql_b_alter;
	sql_b_alter << "ALTER TABLE b ADD COLUMN"
			<< " c_id INTEGER NOT NULL DEFAULT 0"
			<< " CONSTRAINT c_fk REFERENCES c (id)";

	TEST_OK_X(db.query(sql_a), db.logErrors());
	TEST_OK_X(db.query(sql_b), db.logErrors());
	TEST_OK_X(db.query(sql_c), db.logErrors());
	TEST_OK_X(db.query(sql_b_alter), db.logErrors());
	db.commit();
}

bool execStatement (cwt::debby::statement & sth)
{
	bool r;
	TEST_OK_X((r = sth.exec()), sth.logErrors());
	return r;
}

long_t create (cwt::debby::statement & sth)
{
	return execStatement (sth)
			? sth.lastId()
			: 0;
}

long_t persist_data_a (const A & a, cwt::debby::database & db)
{
	pfs::string table_name = _l1("a");
	pfs::string fields     = _l1("ch, sh, i");
	pfs::string wildcards  = _l1("?,?,?");

	pfs::string sql;
	sql << "INSERT INTO " << table_name << "("
		<< fields
		<< ") VALUES (" << wildcards << ")";

	cwt::debby::statement sth = db.prepare(sql)
			.bind(a.ch)
			.bind(a.sh)
			.bind(a.i);

	return create(sth);
}

long_t persist_data_b (const B & b, long_t c_id, cwt::debby::database & db)
{
	pfs::string table_name = _l1("b");
	pfs::string fields     = _l1("text, blob, c_id");
	pfs::string wildcards  = _l1("?,?,?");

	pfs::string sql;
	sql << "INSERT INTO " << table_name << "("
		<< fields
		<< ") VALUES (" << wildcards << ")";

	cwt::debby::statement sth = db.prepare(sql)
			.bind(b.text)
			.bind(b.blob)
			.bind(c_id);

	return create(sth);
}

long_t persist_data_c (const C & c, cwt::debby::database & db)
{
	long_t a_id = persist_data_a(c.a, db);

	pfs::string table_name = _l1("c");
	pfs::string fields     = _l1("a_id");
	pfs::string wildcards  = _l1("?");

	pfs::string sql;
	sql << "INSERT INTO " << table_name << "("
		<< fields
		<< ") VALUES (" << wildcards << ")";

	cwt::debby::statement sth = db.prepare(sql)
			.bind(a_id);

	long_t c_id = create(sth);

	pfs::vector<B>::const_iterator it = c.b.cbegin();
	pfs::vector<B>::const_iterator itEnd = c.b.cend();

	for (; it != itEnd; ++it) {
		TEST_OK(persist_data_b(*it, c_id, db));
	}

	return c_id;
}

void persist_data (cwt::debby::database & db)
{
	A a;
	a.ch = 'A';
	a.sh = pfs::max_type<short int>();
	a.i  = pfs::min_type<int>();

	A a1;
	a1.ch = 'Z';
	a1.sh = pfs::min_type<short int>();
	a1.i  = pfs::max_type<int>();

	B b1;
	b1.text = _l1("Hello");
	b1.blob = pfs::bytearray("World");

	B b2;
	b2.text = _l1("Bye");
	b2.blob = pfs::bytearray("Somebody");

	C c;
	c.a = a1;
	c.b.append(b1);
	c.b.append(b2);

	db.begin();
	bool r = true;
	TEST_OK(r &= persist_data_a(a, db));
	TEST_OK(r &= persist_data_c(c, db));
	db.end(r);
}

int main (int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(20);

	cwt::debby::database::addGlobalSearchPath(pfs::string("."));
	cwt::debby::database::addGlobalSearchPath(pfs::string(".."));
	cwt::debby::database db;
	open_schema(db);
	drop_schema(db);
	create_schema(db);
	persist_data(db);

    END_TESTS;
}



