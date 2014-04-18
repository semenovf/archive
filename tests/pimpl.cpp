/*
 * pimpl.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
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
		_d.swap<implA>(other._d);
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

	TEST_OK(std::string("implA()==implA()implB()==~implA()==~implB()~implA()") == buffer.str());
	std::cout << buffer.str() << std::endl;
}


int main (int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(17);

	test1();
	test2();

	END_TESTS;
}
