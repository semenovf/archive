/**
 * @file nodelist_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODELIST_P_HPP__
#define __CWT_DOM_NODELIST_P_HPP__

#include "../include/cwt/dom/nodelist.hpp"
#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class node_impl;

class DLL_API nodelist_impl
{
public:
	typedef node_impl * item_type;
	typedef pfs::vector<item_type> list_type;

    pfs::atomic_int _ref;

    node_impl * _node_impl;
    pfs::string _tagname;
    pfs::string _nsURI;
    list_type   _list;
    long        _timestamp;

public:
    nodelist_impl (node_impl *);
    nodelist_impl (node_impl *, const pfs::string & );
    nodelist_impl (node_impl *, const pfs::string &, const pfs::string &);
    ~nodelist_impl();
//
//    bool operator == (const nodelist_impl &) const;
//    bool operator != (const nodelist_impl &) const;
//
    void createList ();
    node_impl * item (size_t index);
    size_t length ();
};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_P_HPP__ */
