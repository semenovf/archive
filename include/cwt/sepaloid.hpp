/*
 * sepaloid.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_SEPALOID_HPP__
#define __CWT_SEPALOID_HPP__

#include <cwt/vector.hpp>
#include <cwt/hash.hpp>
#include <cwt/errorable.hpp>
#include <cwt/sigslotmapping.hpp>
#include <cwt/dl.hpp>

CWT_NS_BEGIN

struct PetaloidSpec {
	PetaloidSpec() : p(NULL), ph(NULL), dtor(NULL) {}
	PetaloidSpec(Petaloid *a, Dl::Handle b, petaloid_dtor_t c) : p(a), ph(b), dtor(c) {}
	Petaloid *p;
	Dl::Handle ph;          /* null for local */
	petaloid_dtor_t dtor; /* may be null (no destructor) */
};


class DLL_API Sepaloid : public Errorable
{
public:
	typedef struct { const char *id; sigslot_mapping_t *map; const char *desc; } Mapping;
	typedef Hash<ByteArray, Mapping*> MappingHash;

    class iterator {
    public:
		Vector<PetaloidSpec>::iterator it;

        inline iterator() : it(0)                               { }
        inline iterator(const iterator &o): it(o.it)            { }
        inline iterator(const Vector<PetaloidSpec>::iterator &o): it(o) { }
        inline Petaloid & operator *  () const                  { return *it->p; }
        inline Petaloid * operator -> () const                  { return it->p; }
        inline Petaloid & operator [] (int j) const             { return *it.operator[](j).p; }
        inline            operator Petaloid* () const           { return it->p; }
        inline bool       operator == (const iterator &o) const { return it.operator == (o.it); }
        inline bool       operator != (const iterator &o) const { return it.operator != (o.it); }
        inline bool       operator <  (const iterator &o) const { return it.operator <  (o.it); }
        inline bool       operator <= (const iterator &o) const { return it.operator <= (o.it); }
        inline bool       operator >  (const iterator &o) const { return it.operator >  (o.it); }
        inline bool       operator >= (const iterator &o) const { return it.operator >= (o.it); }
        inline iterator&  operator ++ ()                        { ++it; return *this; }
        inline iterator   operator ++ (int)                     { Vector<PetaloidSpec>::iterator n(it); ++it; return iterator(n); }
        inline iterator&  operator -- ()                        { it--; return *this; }
        inline iterator   operator -- (int)                     { Vector<PetaloidSpec>::iterator n(it); it--; return n; }
        inline iterator&  operator += (int j)                   { it.operator += (j); return *this; }
        inline iterator&  operator -= (int j)                   { it.operator -= (j); return *this; }
        inline iterator   operator +  (int j) const             { return iterator(it.operator + (j)); }
        inline iterator   operator -  (int j) const             { return iterator(it.operator - (j)); }
        inline int        operator -  (iterator j) const        { return it.operator - (j.it); }
    };
    friend class iterator;

    class const_iterator {
    public:
		Vector<PetaloidSpec>::const_iterator it;

        inline const_iterator() : it(0)                               { }
        inline const_iterator(const const_iterator &o): it(o.it)      { }
        inline explicit const_iterator(const iterator &o): it(o.it)   { }
        inline const_iterator(const Vector<PetaloidSpec>::const_iterator &o): it(o) { }
        inline Petaloid & operator *  () const                        { return *it->p; }
        inline Petaloid * operator -> () const                        { return it->p; }
        inline Petaloid & operator [] (int j) const                   { return *it.operator[](j).p; }
        inline            operator Petaloid* () const                 { return it->p; }
        inline bool       operator == (const const_iterator &o) const { return it.operator == (o.it); }
        inline bool       operator != (const const_iterator &o) const { return it.operator != (o.it); }
        inline bool       operator <  (const const_iterator &o) const { return it.operator <  (o.it); }
        inline bool       operator <= (const const_iterator &o) const { return it.operator <= (o.it); }
        inline bool       operator >  (const const_iterator &o) const { return it.operator >  (o.it); }
        inline bool       operator >= (const const_iterator &o) const { return it.operator >= (o.it); }
        inline const_iterator&  operator ++ ()                        { ++it; return *this; }
        inline const_iterator   operator ++ (int)                     { Vector<PetaloidSpec>::const_iterator n(it); ++it; return const_iterator(n); }
        inline const_iterator&  operator -- ()                        { it--; return *this; }
        inline const_iterator   operator -- (int)                     { Vector<PetaloidSpec>::const_iterator n(it); it--; return n; }
        inline const_iterator&  operator += (int j)                   { it.operator += (j); return *this; }
        inline const_iterator&  operator -= (int j)                   { it.operator -= (j); return *this; }
        inline const_iterator   operator +  (int j) const             { return const_iterator(it.operator + (j)); }
        inline const_iterator   operator -  (int j) const             { return const_iterator(it.operator - (j)); }
        inline int              operator -  (const_iterator j) const  { return it.operator - (j.it); }
    };
    friend class const_iterator;

protected:
    Sepaloid() {}

public:
	Sepaloid(Mapping mapping[], int n);
	virtual ~Sepaloid() {
	    disconnectAll();
	    unregisterAll();
	}

	void addSearchPath(const String &dir) { m_searchPaths.append(dir); }
	Petaloid* registerLocalPetaloid(Petaloid &petaloid, petaloid_dtor_t dtor = Petaloid::defaultDtor);
	Petaloid* registerStaticPetaloid(Petaloid &petaloid);
	Petaloid* registerPetaloidForPath(const String &path, const char *pname = NULL, int argc = 0, char **argv = NULL);
	Petaloid* registerPetaloidForName(const String &name, const char *pname = NULL, int argc = 0, char **argv = NULL);

	size_t         count() const  { return m_petaloids.size(); }
    iterator       begin()        { return iterator(m_petaloids.begin()); }
    iterator       end()          { return iterator(m_petaloids.end()); }
    const_iterator begin() const  { return const_iterator(m_petaloids.begin()); }
    const_iterator end() const    { return const_iterator(m_petaloids.end()); }
    const_iterator cbegin() const { return const_iterator(m_petaloids.cbegin()); }
    const_iterator cend() const   { return const_iterator(m_petaloids.cend()); }


/* TODO need implementation
	bool registerPetaloidForUrl(const String &url);
	bool registerPetaloidForUdpSocket(UdpSocket sock);
	bool registerPetaloidForTcpSocket(TcpSocket &sock);
	bool registerPetaloidForLocalSocket(LocalSocket &sock);
*/

	void connectAll();
	void disconnectAll();
	void unregisterAll();

protected:
	bool registerPetaloid(Petaloid &petaloid, Dl::Handle ph, petaloid_dtor_t dtor);

private:
	MappingHash          m_mapping;
	Vector<String>       m_searchPaths;     /* directories where to search petaloids */
	Vector<PetaloidSpec> m_petaloids;
};

CWT_NS_END

#endif /* __CWT_SEPALOID_HPP__ */
