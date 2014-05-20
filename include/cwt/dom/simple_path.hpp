/**
 * @file simple_path.hpp
 * @author wladt
 * @date May 20, 2014
 */

#ifndef __CWT_DOM_SIMPLE_PATH_HPP__
#define __CWT_DOM_SIMPLE_PATH_HPP__

#include <pfs.hpp>

namespace cwt { namespace dom {

class node;
class nodelist;

/*
 * Syntax (according to suggestions from http://goessner.net/articles/JsonPath/):
 * /            root object/element
 * /            child operator
 * .            current object/element
 * ..           parent operator
 * []           subscript operator
 */
class DLL_API simple_path
{
	node _root;

public:
	simple_path (const node & root) : _root(root) {}

//	json_value find (const pfs::string & jpath) const;
//	json_value & find (const pfs::string & jpath);
//	json_value operator [] (const pfs::string & jpath) const { return find(jpath); }
//	json_value operator () (const pfs::string & jpath) const { return operator [] (jpath); }
//	json_value & operator [] (const pfs::string & jpath);
//	json_value & operator () (const pfs::string & jpath) { return operator [] (jpath); }

//	bool contains (const pfs::string & jpath) const { return findValue(jpath) != nullptr; }
};

}} // cwt::dom

#endif /* __CWT_DOM_SIMPLE_PATH_HPP__ */
