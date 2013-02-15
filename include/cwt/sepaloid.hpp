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
#include <cwt/sigslotmapping.hpp>
#include <cwt/dl.hpp>

CWT_NS_BEGIN

struct PetaloidSpec {
	PetaloidSpec() : p(NULL), ph(NULL), dtor(NULL) {}
	PetaloidSpec(Petaloid *a, Dl::Handle b, __petaloid_dtor__ c) : p(a), ph(b), dtor(c) {}
	Petaloid *p;
	Dl::Handle ph;          /* null for local */
	__petaloid_dtor__ dtor; /* may be null (no destructor) */
};


class DLL_API Sepaloid
{
public:
	typedef struct { const char *id; sigslot_mapping_t *map; const char *desc; } Mapping;
	typedef Hash<ByteArray, Mapping*> MappingHash;

public:
	Sepaloid(Mapping mapping[], int n);
	~Sepaloid() {}

	void addSearchPath(const String &dir) { m_searchPaths.append(dir); }
	bool registerLocalPetaloid(Petaloid &petaloid, __petaloid_dtor__ dtor = Petaloid::defaultDtor);
	bool registerStaticPetaloid(Petaloid &petaloid);
	bool registerPetaloidForPath(const String &path);
	bool registerPetaloidForName(const String &name);

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
	bool registerPetaloid(Petaloid &petaloid, Dl::Handle ph, __petaloid_dtor__ dtor);

private:
	MappingHash          m_mapping;
	Vector<String>       m_searchPaths;     /* directories where to search petaloids */
	Vector<PetaloidSpec> m_petaloids;
};

CWT_NS_END

#endif /* __CWT_SEPALOID_HPP__ */
