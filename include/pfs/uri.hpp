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
	uri ()
	{}

	/**
	 * @return URI scheme as string.
	 */
	const string & scheme () const
	{
		return _uri.scheme;
	}

	/**
	 * @return URI authority as string.
	 */
	const string & authority () const
	{
		return _uri.authority;
	}

	/**
	 * @return URI user info as string.
	 */
	const string & userinfo () const
	{
		return _uri.userinfo;
	}

	/**
	 * @return URI host as string.
	 */
	const string & host () const
	{
		return _uri.host;
	}

	/**
	 * @return URI port.
	 */
	uint16_t port () const { return _uri.port; }


	/**
	 * @return URI path as string.
	 */
	const string & path () const
	{
		return _uri.path;
	}

	/**
	 * @return URI query as string.
	 */
	const string & query () const
	{
		return _uri.query;
	}

	/**
	 * @return URI fragment as string.
	 */
	const string & fragment () const
	{
		return _uri.fragment;
	}

	/**
	 * @brief Checks if host represented as IP address (as opposite to DNS name).
	 */
	bool host_is_ip () const
	{
		return _uri.hostIsIp;
	}

	map<string, string> query_items (
		  const string & value_delim = string(1, '=')
		, const string & pair_delim = string(1, '&'));

	bool parse (const string & uri);

	friend string to_string (const uri & u);
};

DLL_API string to_string (const uri & u);

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_URI_HPP__ */
