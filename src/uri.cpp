/*
 * uri.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: wladt
 */

#include <pfs/fsm.hpp>
#include "pfs/uri.hpp"
#include "uri_rfc3986.hpp"

namespace pfs {

void uri_data::clear()
{
	scheme.clear();
	authority.clear();
	userinfo.clear();
	host.clear();
	port = 0;
	path.clear();
	query.clear();
	fragment.clear();
	hostIsIp = false;
}


pfs::map<pfs::string, pfs::string> uri::queryItems(const pfs::string & valueDelimiter, const pfs::string & pairDelimiter)
{
	pfs::map<pfs::string, pfs::string> r;

	pfs::vector<pfs::string> pairs = this->query().split(pairDelimiter);

	pfs::vector<pfs::string>::const_iterator it = pairs.cbegin();
	pfs::vector<pfs::string>::const_iterator itEnd = pairs.cend();

	while (it != itEnd) {
		pfs::vector<pfs::string> pair = it->split(valueDelimiter);
		if (pair.size() > 0) {
			r.insert(pair[0], pair.size() > 1 ? pair[1] : pfs::string());
		}
		++it;
	}

	return r;
}

/**
 * @brief  Parse the given string as URI.
 *
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri pfs::string representation of URI.
 */
bool uri::parse(const pfs::string & uri)
{
	UriParseContext ctx = { & _uri };
	pfs::fsm::fsm<pfs::string> fsm(uri_fsm, & ctx);

	_uri.clear();

	if (fsm.exec(0, uri.begin(), uri.end()) >= 0) {
		return true;
	}
	return false;
}

/**
 *
 * @param uri
 * @return
 */
pfs::string uri::toString() const
{
	pfs::string uri;

	// userinfo without host is an error
	if (!_uri.userinfo.isEmpty() && _uri.host.isEmpty()) {
		return pfs::string(); // null string
	}

	if (!_uri.scheme.isEmpty()) {
		uri += _uri.scheme;
		uri += pfs::string(":");
	}

	if (!_uri.userinfo.isEmpty() || !_uri.host.isEmpty()) {
		uri += pfs::string("//");

		if (!_uri.userinfo.isEmpty()) {
			uri += _uri.userinfo;
			uri += pfs::string("@");
		}

		if (!_uri.host.isEmpty()) {
			uri += _uri.host;
		}

		if (_uri.port > 0) {
			uri += string(":");
			uri += string::toString(_uri.port);
		}

		if (!_uri.path.isEmpty()) {
			uri += _uri.path;
		}

		if (!_uri.query.isEmpty()) {
			uri += pfs::string("?");
			uri += _uri.query;
		}

		if (!_uri.fragment.isEmpty()) {
			uri += pfs::string("#");
			uri += _uri.fragment;
		}
	}

	return uri;
}

} // pfs
