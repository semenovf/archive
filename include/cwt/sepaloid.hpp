/*
 * sepaloid.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_SEPALOID_HPP__
#define __CWT_SEPALOID_HPP__

#include <pfs/mt.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/noncopyable.hpp>
#include <cwt/dl.hpp>
#include <cwt/thread.hpp>
#include <cwt/sigslotmapping.hpp>

namespace cwt {

struct petaloid_spec
{
	petaloid_spec () : p(nullptr), ph(nullptr), dtor(nullptr) {}
	petaloid_spec (petaloid * a, dl::handle b, petaloid_dtor_t c) : p(a), ph(b), dtor(c) {}
	petaloid * p;
	dl::handle ph;        /* null for local */
	petaloid_dtor_t dtor; /* may be null (no destructor) */
};

class DLL_API sepaloid : public cwt::dl, public has_slots<>, pfs::noncopyable
{
	PFS_IMPLEMENT_LOCKING(sepaloid);

public:
	typedef struct { int id; sigslot_mapping_t * map; pfs::string desc; } mapping_type;
	typedef pfs::map<int, mapping_type *> mapping_hash;
	typedef pfs::map<pfs::string, petaloid_spec> petaloid_specs_type;

    class iterator
    {
    public:
		petaloid_specs_type::iterator it;

        inline iterator () : it(0)                              { }
        inline iterator (const iterator & o): it(o.it)          { }
        inline iterator (const petaloid_specs_type::iterator & o): it(o) { }
        inline petaloid & operator *  () const              { return *it->second.p; }
        inline petaloid * operator -> () const              { return it->second.p; }
        //inline petaloid & operator [] (int j) const             { return *it.operator[](j).p; }
        //inline            operator petaloid_spec * () const      { return & it->second; }
        inline bool       operator == (const iterator & o) const { return it == o.it; }
        inline bool       operator != (const iterator & o) const { return it != o.it; }
/*
        inline bool       operator <  (const iterator & o) const { return it <  o.it; }
        inline bool       operator <= (const iterator & o) const { return it <= o.it; }
        inline bool       operator >  (const iterator & o) const { return it >  o.it; }
        inline bool       operator >= (const iterator & o) const { return it >= o.it; }
*/
        inline iterator & operator ++ ()                        { ++it; return *this; }
        inline iterator   operator ++ (int)                     { petaloid_specs_type::iterator n(it); ++it; return iterator(n); }
/*
        inline iterator & operator -- ()                        { it--; return *this; }
        inline iterator   operator -- (int)                     { pfs::vector<petaloid_spec>::iterator n(it); it--; return n; }
        inline iterator & operator += (int j)                   { it.operator += (j); return *this; }
        inline iterator & operator -= (int j)                   { it.operator -= (j); return *this; }
        inline iterator   operator +  (int j) const             { return iterator(it.operator + (j)); }
        inline iterator   operator -  (int j) const             { return iterator(it.operator - (j)); }
        inline intptr_t   operator -  (iterator j) const        { return it - j.it; }
*/
    };
    friend class iterator;

    class const_iterator
    {
    public:
    	petaloid_specs_type::const_iterator it;

        inline const_iterator() : it(0)                               { }
        inline const_iterator(const const_iterator & o): it(o.it)     { }
        inline explicit const_iterator(const iterator &o): it(o.it)   { }
        inline const_iterator(const petaloid_specs_type::const_iterator & o): it(o) { }
        inline const petaloid & operator *  () const              { return *it->second.p; }
        inline const petaloid * operator -> () const              { return it->second.p; }
        //inline petaloid & operator [] (int j) const                   { return *it.operator[](j).p; }
        //inline            operator petaloid_spec * () const            { return & it->second; }
        inline bool       operator == (const const_iterator & o) const { return it == o.it; }
        inline bool       operator != (const const_iterator & o) const { return it != o.it; }
/*
        inline bool       operator <  (const const_iterator & o) const { return it <  o.it; }
        inline bool       operator <= (const const_iterator & o) const { return it <= o.it; }
        inline bool       operator >  (const const_iterator & o) const { return it >  o.it; }
        inline bool       operator >= (const const_iterator & o) const { return it >= o.it; }
*/
        inline const_iterator & operator ++ ()                        { ++it; return *this; }
        inline const_iterator   operator ++ (int)                     { petaloid_specs_type::const_iterator n(it); ++it; return const_iterator(n); }
        inline const_iterator & operator -- ()                        { it--; return *this; }
        inline const_iterator   operator -- (int)                     { petaloid_specs_type::const_iterator n(it); it--; return n; }
/*
        inline const_iterator & operator += (int j)                   { it.operator += (j); return *this; }
        inline const_iterator & operator -= (int j)                   { it.operator -= (j); return *this; }
        inline const_iterator   operator +  (int j) const             { return const_iterator(it.operator + (j)); }
        inline const_iterator   operator -  (int j) const             { return const_iterator(it.operator - (j)); }
        inline intptr_t         operator -  (const_iterator j) const  { return it - j.it; }
*/
    };
    friend class const_iterator;

protected:
    sepaloid() : cwt::dl(), _masterPetaloid(nullptr) {}

public:
	sepaloid (mapping_type mapping[], int n);
	virtual ~sepaloid() {
	    disconnectAll();
	    unregisterAll();
	}

	//void addSearchPath (const pfs::string & dir) { dl::addSearchPath(dir); }
	cwt::petaloid * registerLocalPetaloid (cwt::petaloid * petaloid, petaloid_dtor_t dtor = petaloid::defaultDtor);
	cwt::petaloid * registerPetaloidForPath (const pfs::string & path, const char * pname = nullptr, int argc = 0, char ** argv = nullptr);
	cwt::petaloid * registerPetaloidForName (const pfs::string & name, const char * pname = nullptr, int argc = 0, char ** argv = nullptr);
	void setMasterPetaloid (petaloid *p) { _masterPetaloid = p; }
	size_t         count () const  { return _petaloids.size(); }
    iterator       begin ()        { return iterator(_petaloids.begin()); }
    iterator       end   ()        { return iterator(_petaloids.end()); }
    const_iterator begin () const  { return const_iterator(_petaloids.begin()); }
    const_iterator end   () const  { return const_iterator(_petaloids.end()); }
    const_iterator cbegin() const  { return const_iterator(_petaloids.cbegin()); }
    const_iterator cend  () const  { return const_iterator(_petaloids.cend()); }

/* TODO need implementation
	bool registerPetaloidForUrl(const pfs::string &url);
*/
	void connectAll ();
	void disconnectAll ();
	void unregisterAll ();
	bool start ();
	int  exec ();

	bool isPetaloidRegistered (const pfs::string & pname)
		{ return _petaloids.contains(pname); }

public: /*slots*/
	void onPetaloidRegistered (const pfs::string & pname, bool & result)
	{
		result = isPetaloidRegistered (pname);
	}

protected:
	bool registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor);

private:
	mapping_hash               _mapping;
	petaloid_specs_type        _petaloids;
	pfs::vector<cwt::thread *> _threads;
	petaloid *                 _masterPetaloid;
};

} // cwt

#endif /* __CWT_SEPALOID_HPP__ */
