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

class DLL_API sepaloid : public cwt::dl, public has_slots<>
{
	PFS_DENY_COPY(sepaloid)
	PFS_IMPLEMENT_LOCKING(sepaloid);

public:
	typedef struct { int id; sigslot_mapping_t * map; const char * desc; } mapping_type;
	typedef pfs::map<int, mapping_type *> mapping_hash;

    class iterator
    {
    public:
		pfs::vector<petaloid_spec>::iterator it;

        inline iterator() : it(0)                               { }
        inline iterator(const iterator &o): it(o.it)            { }
        inline iterator(const pfs::vector<petaloid_spec>::iterator &o): it(o) { }
        inline petaloid & operator *  () const                  { return *it->p; }
        inline petaloid * operator -> () const                  { return it->p; }
        inline petaloid & operator [] (int j) const             { return *it.operator[](j).p; }
        inline            operator petaloid * () const           { return it->p; }
        inline bool       operator == (const iterator & o) const { return it == o.it; }
        inline bool       operator != (const iterator & o) const { return it != o.it; }
        inline bool       operator <  (const iterator & o) const { return it <  o.it; }
        inline bool       operator <= (const iterator & o) const { return it <= o.it; }
        inline bool       operator >  (const iterator & o) const { return it >  o.it; }
        inline bool       operator >= (const iterator & o) const { return it >= o.it; }
        inline iterator & operator ++ ()                        { ++it; return *this; }
        inline iterator   operator ++ (int)                     { pfs::vector<petaloid_spec>::iterator n(it); ++it; return iterator(n); }
        inline iterator & operator -- ()                        { it--; return *this; }
        inline iterator   operator -- (int)                     { pfs::vector<petaloid_spec>::iterator n(it); it--; return n; }
        inline iterator & operator += (int j)                   { it.operator += (j); return *this; }
        inline iterator & operator -= (int j)                   { it.operator -= (j); return *this; }
        inline iterator   operator +  (int j) const             { return iterator(it.operator + (j)); }
        inline iterator   operator -  (int j) const             { return iterator(it.operator - (j)); }
        inline intptr_t   operator -  (iterator j) const        { return it - j.it; }
    };
    friend class iterator;

    class const_iterator
    {
    public:
		pfs::vector<petaloid_spec>::const_iterator it;

        inline const_iterator() : it(0)                               { }
        inline const_iterator(const const_iterator &o): it(o.it)      { }
        inline explicit const_iterator(const iterator &o): it(o.it)   { }
        inline const_iterator(const pfs::vector<petaloid_spec>::const_iterator &o): it(o) { }
        inline petaloid & operator *  () const                        { return *it->p; }
        inline petaloid * operator -> () const                        { return it->p; }
        inline petaloid & operator [] (int j) const                   { return *it.operator[](j).p; }
        inline            operator petaloid * () const                { return it->p; }
        inline bool       operator == (const const_iterator & o) const { return it == o.it; }
        inline bool       operator != (const const_iterator & o) const { return it != o.it; }
        inline bool       operator <  (const const_iterator & o) const { return it <  o.it; }
        inline bool       operator <= (const const_iterator & o) const { return it <= o.it; }
        inline bool       operator >  (const const_iterator & o) const { return it >  o.it; }
        inline bool       operator >= (const const_iterator & o) const { return it >= o.it; }
        inline const_iterator & operator ++ ()                        { ++it; return *this; }
        inline const_iterator   operator ++ (int)                     { pfs::vector<petaloid_spec>::const_iterator n(it); ++it; return const_iterator(n); }
        inline const_iterator & operator -- ()                        { it--; return *this; }
        inline const_iterator   operator -- (int)                     { pfs::vector<petaloid_spec>::const_iterator n(it); it--; return n; }
        inline const_iterator & operator += (int j)                   { it.operator += (j); return *this; }
        inline const_iterator & operator -= (int j)                   { it.operator -= (j); return *this; }
        inline const_iterator   operator +  (int j) const             { return const_iterator(it.operator + (j)); }
        inline const_iterator   operator -  (int j) const             { return const_iterator(it.operator - (j)); }
        inline intptr_t         operator -  (const_iterator j) const  { return it - j.it; }
    };
    friend class const_iterator;

protected:
    sepaloid() : cwt::dl(), m_masterPetaloid(nullptr) {}

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
	void setMasterPetaloid (petaloid *p) { m_masterPetaloid = p; }
	size_t         count () const  { return m_petaloids.size(); }
    iterator       begin ()        { return iterator(m_petaloids.begin()); }
    iterator       end   ()        { return iterator(m_petaloids.end()); }
    const_iterator begin () const  { return const_iterator(m_petaloids.begin()); }
    const_iterator end   () const  { return const_iterator(m_petaloids.end()); }
    const_iterator cbegin() const  { return const_iterator(m_petaloids.cbegin()); }
    const_iterator cend  () const  { return const_iterator(m_petaloids.cend()); }

/* TODO need implementation
	bool registerPetaloidForUrl(const pfs::string &url);
*/
	void connectAll ();
	void disconnectAll ();
	void unregisterAll ();
	void start ();
	int  exec ();

// slots
	//void quit();

protected:
	bool registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor);

private:
	mapping_hash               m_mapping;
	pfs::vector<petaloid_spec> m_petaloids;
	pfs::vector<cwt::thread *> m_threads;
	petaloid *                 m_masterPetaloid;
};

} // cwt

#endif /* __CWT_SEPALOID_HPP__ */
