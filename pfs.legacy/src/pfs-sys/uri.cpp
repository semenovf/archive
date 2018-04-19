/*
 * uri.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: wladt
 */

#include <pfs/fsm.hpp>
#include <pfs/stringlist.hpp>
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


map<string, string> uri::query_items (
		  const string & value_delim
		, const string & pair_delim)
{
	map<string, string> r;

	stringlist pairs;
	split(this->query(), pair_delim, true, pairs);

	stringlist::const_iterator it = pairs.cbegin();
	stringlist::const_iterator itEnd = pairs.cend();

	while (it != itEnd) {
		stringlist pair;
		split(*it, value_delim, true, pair);

		if (!pair.empty()) {
			r.insert(map<string, string>::value_type(pair[0], pair.size() > 1 ? pair[1] : string()));
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
bool uri::parse (const pfs::string & uri)
{
	parse_context ctx = { & _uri };
	fsm::fsm<pfs::string> fsm(uri_fsm, & ctx);

	_uri.clear();

	fsm::fsm<pfs::string>::result_type r = fsm.exec(0, uri.begin(), uri.end());

	if (r.first)
		return true;

	return false;
}

/**
 *
 * @param uri
 * @return
 */
string to_string (const uri & u)
{
	string r;

	// userinfo without host is an error
	//
	if (!u._uri.userinfo.empty() && u._uri.host.empty()) {
		return string(); // null string
	}

	if (!u._uri.scheme.empty()) {
		r.append(u._uri.scheme);
		r.append(_u8(":"));
	}

	if (!u._uri.userinfo.empty() || !u._uri.host.empty()) {
		r.append(_u8("//"));

		if (!u._uri.userinfo.empty()) {
			r.append(u._uri.userinfo);
			r.append(_u8("@"));
		}

		if (!u._uri.host.empty()) {
			r.append(u._uri.host);
		}

		if (u._uri.port > 0) {
			r.append(_u8(":"));
			r.append(to_string(u._uri.port));
		}

		if (!u._uri.path.empty()) {
			r.append(u._uri.path);
		}

		if (!u._uri.query.empty()) {
			r.append(_u8("?"));
			r.append(u._uri.query);
		}

		if (!u._uri.fragment.empty()) {
			r.append(_u8("#"));
			r.append(u._uri.fragment);
		}
	}

	return r;
}

} // pfs
