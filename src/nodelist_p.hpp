/**
 * @file nodelist_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NODELIST_P_HPP__
#define __CWT_DOM_NODELIST_P_HPP__

#include "../include/cwt/dom/nodelist.hpp"
#include <pfs/vector.hpp>
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class node::impl;

class nodelist::impl
{
	typedef node::impl * item_type;
	typedef pfs::vector<item_type> list_type;

    pfs::atomic_integer ref;

    /*
      This list contains the children of this node.
     */
    node::impl * node_impl;
    pfs::string tagname;
    pfs::string nsURI;
    list_type list;
    long timestamp;

public:
    impl (node::impl*);
    impl (node::impl*, const pfs::string& );
    impl (node::impl*, const pfs::string&, const pfs::string& );
    ~impl();

    bool operator== (const impl&) const;
    bool operator!= (const impl&) const;

    void createList();
    node::impl* item(int index);
    int length() const;

};

}} // cwt::dom

#endif /* __CWT_DOM_NODELIST_P_HPP__ */
