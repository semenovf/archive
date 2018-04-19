/*
 * pimpl.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/pimpl.hpp>
#include <cstring>
#include "pimpl_test.hpp"
#include <iostream>
#include <sstream>

void test1 ()
{
	A a;
	a.setChar('W');
	TEST_OK(a.getChar() == 'W');
	a.setInt(256);
	TEST_OK(a.getInt() == 256);
	a.setDouble(3.14159f);
	TEST_OK(a.getDouble() == double(3.14159f));

	B b;
	b.setChar('Q');
	TEST_OK(b.getChar() == 'Q');
	b.setInt(312);
	TEST_OK(b.getInt() == 312);
	b.setDouble(1.234567f);
	TEST_OK(b.getDouble() == double(1.234567f));

	b.setString("Hello, World!");
	TEST_OK(strcmp("Hello, World!", b.getString()) == 0);
	TEST_OK(b.getChar() == 'Q');

	B b1 = b;
	TEST_OK(b1.getChar() == 'Q');
	TEST_OK(b1.getInt() == 312);
	TEST_OK(b1.getDouble() == double(1.234567f));
	TEST_OK(strcmp("Hello, World!", b1.getString()) == 0);

	C c(b);
	TEST_OK(strcmp("Hello, World!", c.getString()) == 0);
	c.setString("Hello, World, again!"); // COW

	b.setString("By, World!");
	TEST_OK(strcmp("By, World!", b.getString()) == 0);
	TEST_OK(strcmp("By, World!", b1.getString()) == 0);
	TEST_OK(strcmp("Hello, World, again!", c.getString()) == 0);
}


struct implA
{
	std::ostream & _os;
	implA (std::ostream & os) : _os(os) { os << "implA()"; }
	virtual ~implA () { _os << "~implA()"; }
};

struct implB : public implA
{
	implB (std::ostream & os) : implA(os) { os << "implB()"; }
	virtual ~implB () { _os << "~implB()"; }
};

struct AA
{
	pfs::pimpl _d;
	AA (std::ostream & os) : _d(new implA(os)) {}
	AA (std::ostream & os, int) : _d(new implB(os)) {}
	void swap (AA & other)
	{
		_d.swap(other._d);
	}
};

void test2 ()
{
	std::stringbuf buffer;
	std::ostream os (& buffer);

	AA * a  = new AA(os);   // implA()
	os << "==";

	AA * b = new AA(os,1); // implA()implB()
	os << "==";

	a->swap(*b);

	delete b; // ~implA()

	os << "==";

	delete a; //~implB()~implA()
	std::cout << buffer.str() << std::endl;
	TEST_OK(std::string("implA()==implA()implB()==~implA()==~implB()~implA()") == buffer.str());
}


struct Int
{
	int _i;
	Int () : _i(0) {}
	Int (int i) : _i(i) {}
};

void test0 ()
{
	{
		pfs::nullable<Int> nil1;
		pfs::nullable<Int> nil2;

		TEST_OK(nil1.isNull());
		TEST_OK(nil2.isNull());

		nil1.swap(nil2);

		TEST_OK(nil1.isNull());
		TEST_OK(nil2.isNull());
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i(new Int(123));

		TEST_FAIL(i.cast()->_i == 123);

		nil.swap(i);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(i1);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);

		nil.swap(i1);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i1.isNull());
		TEST_FAIL(!i2.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(i1);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);

		nil.swap(i2);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i2.isNull());
		TEST_FAIL(!i1.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
		TEST_FAIL(i1.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i(new Int(123));

		TEST_FAIL(i.cast()->_i == 123);

		i.swap(nil);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(i1);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);

		i1.swap(nil);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i1.isNull());
		TEST_FAIL(!i2.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> nil;
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(i1);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);

		i2.swap(nil);

		TEST_FAIL(!nil.isNull());
		TEST_FAIL(i2.isNull());
		TEST_FAIL(!i1.isNull());
		TEST_FAIL(nil.cast()->_i == 123);
		TEST_FAIL(i1.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(new Int(456));

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 456);

		i1.swap(i2);

		TEST_FAIL(i1.cast()->_i == 456);
		TEST_FAIL(i2.cast()->_i == 123);
	}

	{
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(new Int(456));
		pfs::nullable<Int> i3(i1);
		pfs::nullable<Int> i4(i2);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i3.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 456);
		TEST_FAIL(i4.cast()->_i == 456);

		i1.swap(i2);

		TEST_FAIL(i1.cast()->_i == 456);
		TEST_FAIL(i3.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 123);
		TEST_FAIL(i4.cast()->_i == 456);
	}

	{
		pfs::nullable<Int> i1(new Int(123));
		pfs::nullable<Int> i2(new Int(456));
		pfs::nullable<Int> i3(i1);
		pfs::nullable<Int> i4(i2);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i3.cast()->_i == 123);
		TEST_FAIL(i2.cast()->_i == 456);
		TEST_FAIL(i4.cast()->_i == 456);

		i3.swap(i4);

		TEST_FAIL(i1.cast()->_i == 123);
		TEST_FAIL(i3.cast()->_i == 456);
		TEST_FAIL(i2.cast()->_i == 456);
		TEST_FAIL(i4.cast()->_i == 123);
	}
}

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(77);

	test1();
	test2();
	test0();

	return END_TESTS;
}
