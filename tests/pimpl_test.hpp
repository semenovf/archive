/**
 * @file pimpl.hpp
 * @author wladt
 * @date Mar 17, 2014
 */

#ifndef __PFS_PIMPL_TEST_HPP__
#define __PFS_PIMPL_TEST_HPP__

#include <pfs/pimpl.hpp>

class A
{
protected:
	class Impl;
	pfs::pimpl _d;

	// if has derived class and it has own Impl class
	template <typename T>
	A (T * p) : _d(p) {}

public:
	A ();

	char   getChar () const;
	int    getInt () const;
	double getDouble () const;

	void setChar (char ch);
	void setInt (int i);
	void setDouble (double d);
};

class B : public A
{
protected:
	class Impl;

	// if has derived class and it has own Impl class
	template <typename T>
	B (const T & p) : A(p) {}

public:
	B ();
	const char * getString () const;
	virtual void setString (const char * s);
};

class C : public B
{
public:
	C ();
	C (const B & b);
	C (const C & c);
	virtual void setString (const char * s);
};

#endif /* __PFS_PIMPL_TEST_HPP__ */
