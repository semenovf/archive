/**
 * @file   uri.h
 * @author wladt
 * @date   Jan 31, 2013 11:27:00 AM
 *
 * @brief
 */

#ifndef __PFS_URI_HPP__
#define __PFS_URI_HPP__

#include <pfs/string.hpp>
#include <pfs/map.hpp>

namespace pfs {

/* C#   System.Uri
 * Java java.net.URI
 * Qt   QUrl, QUrlQuery (for query string parsing)
 */
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

struct DLL_API uri_data
{
	uri_data() : port(0), hostIsIp(false) {}
	void clear();

	pfs::string scheme;
	/*pfs::string m_schemeSpecificPart;*/
	pfs::string authority;
	pfs::string userinfo;
	pfs::string host;
	uint16_t    port;
	pfs::string path;
	pfs::string query;
	pfs::string fragment;
	bool        hostIsIp;
};

class DLL_API uri
{
	uri_data _uri;

public:
	uri () {}
	uri (const pfs::string &uri) { parse(uri); }

	const pfs::string & scheme () const { return _uri.scheme; }
	const pfs::string & authority () const { return _uri.authority; }
	const pfs::string & userinfo () const { return _uri.userinfo; }
	const pfs::string & host () const { return _uri.host; }
	uint16_t port () const { return _uri.port; }
	const pfs::string & path () const { return _uri.path; }
	const pfs::string & query () const { return _uri.query; }
	const pfs::string & fragment () const { return _uri.fragment; }
	bool hostIsIp () const { return _uri.hostIsIp; }

	pfs::map<pfs::string, pfs::string> queryItems (const pfs::string & valueDelimiter = pfs::string("="), const pfs::string & pairDelimiter = pfs::string("&"));

	bool parse (const pfs::string & uri);
	pfs::string toString () const;
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_URI_HPP__ */
