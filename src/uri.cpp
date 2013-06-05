/*
 * uri.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: wladt
 */

#include <cwt/uri.hpp>
#include <cwt/fsm.hpp>

#include <cwt/uri.hpp>
#include "uri_rfc3986.hpp"

CWT_NS_BEGIN

void UriData::clear()
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

/**
 * @brief  Parse the given string as URI.
 *
 * @details Implementation of RFC 3986 (http://tools.ietf.org/html/rfc3986)
 *
 * @param uri String representation of URI.
 */
bool Uri::parse(const String &uri)
{
	UriParseContext ctx = { this, &m_uri };
	Fsm<Char> fsm(uri_fsm, &ctx);

	m_uri.clear();

	if (fsm.exec(0, uri.data(), uri.length()) >= 0) {
		clearErrors();
		return true;
	}
	return false;
}

/**
 *
 * @param uri
 * @return
 */
String Uri::toString() const
{
	String uri;

	// userinfo without host is an error
	if (!m_uri.userinfo.isEmpty() && m_uri.host.isEmpty()) {
		return String(); // null string
	}

	if (!m_uri.scheme.isEmpty()) {
		uri += m_uri.scheme;
		uri += _U(":");
	}

	if (!m_uri.userinfo.isEmpty() || !m_uri.host.isEmpty()) {
		uri += _U("//");

		if (!m_uri.userinfo.isEmpty()) {
			uri += m_uri.userinfo;
			uri += _U("@");
		}

		if (!m_uri.host.isEmpty()) {
			uri += m_uri.host;
		}

		if (m_uri.port > 0) {
			uri += _U(":");
			uri += String::number(m_uri.port);
		}

		if (!m_uri.path.isEmpty()) {
			uri += m_uri.path;
		}

		if (!m_uri.query.isEmpty()) {
			uri += _U("?");
			uri += m_uri.query;
		}

		if (!m_uri.fragment.isEmpty()) {
			uri += _U("#");
			uri += m_uri.fragment;
		}
	}

	return uri;
}

CWT_NS_END
