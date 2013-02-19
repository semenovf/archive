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
	PetaloidSpec(Petaloid *a, Dl::Handle b, petaloid_dtor_t c) : p(a), ph(b), dtor(c) {}
	Petaloid *p;
	Dl::Handle ph;          /* null for local */
	petaloid_dtor_t dtor; /* may be null (no destructor) */
};


class DLL_API Sepaloid
{
public:
	typedef struct { const char *id; sigslot_mapping_t *map; const char *desc; } Mapping;
	typedef Hash<ByteArray, Mapping*> MappingHash;

public:
	Sepaloid(Mapping mapping[], int n);
	~Sepaloid() {
	    disconnectAll();
	    unregisterAll();
	}

	void addSearchPath(const String &dir) { m_searchPaths.append(dir); }
	Petaloid* registerLocalPetaloid(Petaloid &petaloid, petaloid_dtor_t dtor = Petaloid::defaultDtor);
	Petaloid* registerStaticPetaloid(Petaloid &petaloid);
	Petaloid* registerPetaloidForPath(const String &path, const char *pname = NULL, int argc = 0, char **argv = NULL);
	Petaloid* registerPetaloidForName(const String &name, const char *pname = NULL, int argc = 0, char **argv = NULL);

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
