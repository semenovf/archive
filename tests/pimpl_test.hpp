/**
 * @file pimpl.hpp
 * @author wladt
 * @date Mar 17, 2014
 */

#ifndef __PFS_PIMPL_TEST_HPP__
#define __PFS_PIMPL_TEST_HPP__

#include "pimpl_test.hpp"
#include <pfs/shared_ptr.hpp>

template <typename ImplClass>
class pimpl
{
	pfs::shared_ptr<ImplClass> _p;

public:
	pimpl (ImplClass * p) : _p(p) {}

	template <typename ImplClass1>
	ImplClass1 & cast () { return *static_cast<ImplClass1 *>(_p.get()); }

	template <typename ImplClass1>
	const ImplClass1 & cast () const { return *static_cast<ImplClass1 *>(_p.get()); }

	template <typename ImplClass1>
	ImplClass1 & assign () { detach<ImplClass1>(); return *static_cast<ImplClass1 *>(_p.get()); }

	template <typename ImplClass1>
	void detach ()
	{
		if (!_p.unique()) {
			pfs::shared_ptr<ImplClass> p(new ImplClass1(*_p));
			_p.swap(p);
		}
	}

};

class A
{
protected:
	class Impl;

protected:
	pimpl<Impl> _d;

protected:
	A (Impl * p);

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

	virtual void setString (const char * s) override;
};

#endif /* __PFS_PIMPL_TEST_HPP__ */
