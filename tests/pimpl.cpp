/*
 * pimpl.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/pimpl.hpp>
#include <cstring>

class A
{
public:
	class Impl;

protected:
	pfs::pimpl<Impl> _d;

public:
	A () {}
	bool isNull() const { return _d.isNull(); }

	char   getChar () const;
	int    getInt () const;
	double getDouble () const;

	void setChar (char ch);
	void setInt (int i);
	void setDouble (double d);
};

class B : public A
{
public:
	class Impl;

public:
	B () : A() {}
	const char * getString () const;
	void setString (const char * s);
};

class A::Impl
{
public:
	char   ch;
	int    i;
	double d;
};

class B::Impl : public A::Impl
{
public:
	const char * str;
};

char   A::getChar   () const   { return _d->ch; }
int    A::getInt    () const   { return _d->i; }
double A::getDouble () const   { return _d->d; }
void   A::setChar   (char ch)  { _d->ch = ch; }
void   A::setInt    (int i)    { _d->i = i; }
void   A::setDouble (double d) { _d->d = d; }

const char * B::getString () const         { return _d.cast<B::Impl>().str; }
void         B::setString (const char * s) { _d.assign<B::Impl>().str = s; }

void test1 ()
{
	A a;
	TEST_OK(a.isNull());
	a.setChar('W');
	TEST_OK(a.getChar() == 'W');
	a.setInt(256);
	TEST_OK(a.getInt() == 256);
	a.setDouble(3.14159f);
	TEST_OK(a.getDouble() == double(3.14159f));

	B b;
	TEST_OK(b.isNull());
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
	TEST_OK(!b1.isNull());
	TEST_OK(b1.getChar() == 'Q');
	TEST_OK(b1.getInt() == 312);
	TEST_OK(b1.getDouble() == double(1.234567f));
	TEST_OK(strcmp("Hello, World!", b1.getString()) == 0);
}

int main (int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(9);

	test1();

	END_TESTS;
}
