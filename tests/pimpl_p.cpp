/**
 * @file pimpl_p.cpp
 * @author wladt
 * @date Mar 17, 2014
 */

#include "pimpl_test.hpp"

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
	Impl () : A::Impl() {}
	Impl (const A::Impl & o) : A::Impl(o), str(0) {}
};

A::A (Impl * p) : _d(p) {}
A::A () : _d(new A::Impl) {}
char   A::getChar   () const   { return _d.cast<A::Impl>().ch; }
int    A::getInt    () const   { return _d.cast<A::Impl>().i; }
double A::getDouble () const   { return _d.cast<A::Impl>().d; }
void   A::setChar   (char ch)  { _d.cast<A::Impl>().ch = ch; }
void   A::setInt    (int i)    { _d.cast<A::Impl>().i = i; }
void   A::setDouble (double d) { _d.cast<A::Impl>().d = d; }

B::B () : A(new B::Impl) {}
const char * B::getString () const         { return _d.cast<B::Impl>().str; }
void         B::setString (const char * s) { _d.cast<B::Impl>().str = s; }

C::C () : B() {}
C::C (const B & b) : B(b) { /*_d = b._d; */}
C::C (const C & c) : B(c) { /*_d = c._d; */}
void C::setString (const char * s) { _d.assign<B::Impl>().str = s; }
