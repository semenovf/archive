/*
 * sepaloid.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __PFS_SEPALOID_HPP__
#define __PFS_SEPALOID_HPP__

#include <pfs/mt.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/noncopyable.hpp>
#include <pfs/dl.hpp>
#include <pfs/thread.hpp>
#include <pfs/sigslotmapping.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct petaloid_spec
{
	petaloid_spec () : p(nullptr), ph(nullptr), dtor(nullptr) {}
	petaloid_spec (petaloid * a, dl::handle b, petaloid_dtor_t c) : p(a), ph(b), dtor(c) {}
	petaloid * p;
	dl::handle ph;        /* null for local */
	petaloid_dtor_t dtor; /* may be null (no destructor) */
};

class DLL_API sepaloid : public dl, public has_slots<>, noncopyable
{
	PFS_IMPLEMENT_LOCKING(sepaloid);

public:
	typedef struct { int id; sigslot_mapping_t * map; string desc; } mapping_type;
	typedef map<int, mapping_type *> mapping_hash;
	typedef map<string, petaloid_spec> petaloid_specs_type;

private:
	mapping_hash        _mapping;
	petaloid_specs_type _petaloids;
	vector<thread *>    _threads;
	petaloid *          _masterPetaloid;

public:
//    class iterator
//    {
//    public:
//		petaloid_specs_type::iterator it;
//
//        inline iterator () : it()  {}
//        inline iterator (const iterator & o): it(o.it) {}
//        inline iterator (const petaloid_specs_type::iterator & o): it(o) { }
//        inline petaloid & operator *  () const                   { return it.valueRef(); }
//        inline petaloid * operator -> () const                   { return it->second.p; }
//        inline bool       operator == (const iterator & o) const { return it == o.it; }
//        inline bool       operator != (const iterator & o) const { return it != o.it; }
//        inline iterator & operator ++ ()                         { ++it; return *this; }
//        inline iterator   operator ++ (int)                      { petaloid_specs_type::iterator n(it); ++it; return iterator(n); }
//    };
//    friend class iterator;
//
//    class const_iterator
//    {
//    public:
//    	petaloid_specs_type::const_iterator it;
//
//        inline const_iterator() : it() {}
//        inline const_iterator(const const_iterator & o): it(o.it)     {}
//        inline explicit const_iterator(const iterator &o): it(o.it)   {}
//        inline const_iterator(const petaloid_specs_type::const_iterator & o): it(o) {}
//        inline const petaloid & operator *  () const              { return it.valueRef(); }
//        inline const petaloid * operator -> () const              { return it->second.p; }
//        inline bool       operator == (const const_iterator & o) const { return it == o.it; }
//        inline bool       operator != (const const_iterator & o) const { return it != o.it; }
//        inline const_iterator & operator ++ ()    { ++it; return *this; }
//        inline const_iterator   operator ++ (int) { petaloid_specs_type::const_iterator n(it); ++it; return const_iterator(n); }
//        inline const_iterator & operator -- ()    { it--; return *this; }
//        inline const_iterator   operator -- (int) { petaloid_specs_type::const_iterator n(it); it--; return n; }
//    };
//    friend class const_iterator;

protected:
    sepaloid() : dl(), _masterPetaloid(nullptr) {}

public:
	sepaloid (mapping_type mapping[], int n);
	virtual ~sepaloid() {
	    disconnectAll();
	    unregisterAll();
	}

	//void addSearchPath (const string & dir) { dl::addSearchPath(dir); }
	petaloid * registerLocalPetaloid (petaloid * petaloid, petaloid_dtor_t dtor = petaloid::defaultDtor);
	petaloid * registerPetaloidForPath (const string & path, const char * pname = nullptr, int argc = 0, char ** argv = nullptr);
	petaloid * registerPetaloidForName (const string & name, const char * pname = nullptr, int argc = 0, char ** argv = nullptr);
	void setMasterPetaloid (petaloid * p) { _masterPetaloid = p; }
	size_t count () const  { return _petaloids.size(); }
//    iterator       begin ()        { return iterator(_petaloids.begin()); }
//    iterator       end   ()        { return iterator(_petaloids.end()); }
//    const_iterator begin () const  { return const_iterator(_petaloids.begin()); }
//    const_iterator end   () const  { return const_iterator(_petaloids.end()); }
//    const_iterator cbegin() const  { return const_iterator(_petaloids.cbegin()); }
//    const_iterator cend  () const  { return const_iterator(_petaloids.cend()); }

/* TODO need implementation
	bool registerPetaloidForUrl(const string &url);
*/
	void connectAll ();
	void disconnectAll ();
	void unregisterAll ();
	bool start ();
	int  exec ();

	bool isPetaloidRegistered (const string & pname)
		{ return _petaloids.contains(pname); }

public: /*slots*/
	void onPetaloidRegistered (const string & pname, bool & result)
	{
		result = isPetaloidRegistered (pname);
	}

protected:
	bool registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_SEPALOID_HPP__ */
