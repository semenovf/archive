/*
 * sepaloid.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_UI_SEPALOID_HPP__
#define __CWT_UI_SEPALOID_HPP__

#include <cwt/vector.hpp>
#include <cwt/hash.hpp>
#include <cwt/sigslotmapping.hpp>

CWT_NS_BEGIN

class Sepaloid
{
public:
	typedef struct { const char *id; sigslot_mapping_t *map; const char *desc; } Mapping;
	typedef Hash<const char*, Mapping*> MappingHash;

public:
	Sepaloid(Mapping mapping[], int n);
	~Sepaloid() {}

	void appendExternPetaloidDir(const String &dir) { m_searchDirs.append(dir); }
	bool registerLocalPetaloid(Petaloid &petaloid);


/* TODO need implementation
	bool registerPetaloidForPath(const String &path);
	bool registerPetaloidForName(const String &name);
*/

/* TODO need implementation
	bool registerPetaloidForUrl(const String &url);
	bool registerPetaloidForUdpSocket(UdpSocket sock);
	bool registerPetaloidForTcpSocket(TcpSocket &sock);
	bool registerPetaloidForLocalSocket(LocalSocket &sock);
*/

	void connectAll();
/*
	bool loadPetaloids(
			void (*mapping_proc)(SepaloidBootstrap&),
			unsigned int n, String specs[],
			unsigned int l, Petaloid *locals[]);

	void foreach_petaloid( void (*)(Petaloid*) );
*/

protected:
/*
	Petaloid* loadPetaloid(const String &name, const String &dir);
	void registerPetaloid(Petaloid *p, SepaloidBootstrap *bootstrap);
*/

public /*signals*/:
/*
	sigslot::signal0<>              emitPetaloidDestroy;
	sigslot::signal1<const String&> emitPetaloidLoaded;
*/

private /*signals*/:
/*
	sigslot::signal0<> emitPetaloidRegisteredSys;
	sigslot::signal0<> emitPetaloidRegistered;
*/

private:
	MappingHash     m_mapping;
	Vector<String>  m_searchDirs;     /* directories where to find petaloids */
	/*Vector<Petaloid*> m_petaloids;*/
};

CWT_NS_END

#endif /* __CWT_UI_SEPALOID_HPP__ */
