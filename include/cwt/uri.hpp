/**
 * @file   uri.h
 * @author wladt
 * @date   Jan 31, 2013 11:27:00 AM
 *
 * @brief
 */

#ifndef __CWT_URI_HPP__
#define __CWT_URI_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/errorable.hpp>

CWT_NS_BEGIN

/* C#   System.Uri
 * Java java.net.URI
 * Qt   QUrl, QUrlQuery (for query string parsing)
 */

struct DLL_API UriData {
	UriData() : port(0), hostIsIp(false) {}
	void clear();

	String   scheme;
	/*String m_schemeSpecificPart;*/
	String   authority;
	String   userinfo;
	String   host;
	uint16_t port;
	String   path;
	String   query;
	String   fragment;
	bool     hostIsIp;
};

class DLL_API Uri : public Errorable {
public:
	Uri() : Errorable() {}
	Uri(const String &uri) : Errorable() { parse(uri); }

	const String& scheme() const { return m_uri.scheme; }
	const String& authority() const { return m_uri.authority; }
	const String& userinfo() const { return m_uri.userinfo; }
	const String& host() const { return m_uri.host; }
	uint16_t port() const { return m_uri.port; }
	const String& path() const { return m_uri.path; }
	const String& query() const { return m_uri.query; }
	const String& fragment() const { return m_uri.fragment; }
	bool hostIsIp() const { return m_uri.hostIsIp; }

	bool isValid() const { return isGood(); }
	bool parse(const String &uri);
	String toString() const;
private:
	UriData m_uri;
};

struct UriParseContext {
	Errorable *status;
	UriData   *uridata;
};

CWT_NS_END

#endif /* __CWT_URI_HPP__ */
