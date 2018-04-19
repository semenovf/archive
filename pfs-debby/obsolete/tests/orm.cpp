/**
 * @file orm.cpp
 * @author wladt
 * @date Apr 4, 2014
 */


#include <cwt/test.hpp>
#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/vector.hpp>
#include <pfs/map.hpp>
#include <cwt/fs.hpp>
#include <cwt/debby/schema.hpp>
#include <iostream>

// XXX Insertion in Sqlite3 is very slow (one transaction for each insertion), so it is good idea
//     to wrap series of insertions with transaction.

struct A
{
	int  id;
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
	pfs::map<pfs::string,B> c;
};

int main (int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(1);

	cwt::debby::schema schema;

	cwt::debby::table<A> t0(_l1("A"), schema);
	cwt::debby::field_basic & f = t0.addField(_l1("id"), t0().id);
	f.setPk();
	t0.addField(_l1("ch"), t0().ch);
	t0.addField(_l1("sh"), t0().sh);
	t0.addField(_l1("i"), t0().i);

	cwt::debby::table<B> t1(_l1("B"), schema);
	t1.addField(_l1("text"), t1().text);
	t1.addField(_l1("blob"), t1().blob);

	t1().blob = pfs::bytearray("blob");
	t1().text = pfs::string("text");
	t1.persist();

	cwt::debby::table<C> t2(_l1("C"), schema);
	//t1.addField(c.a);
	t2.addField(_l1("b"), t2().b);
	t2.addField(_l1("c"), t2().c);

    END_TESTS;
}





