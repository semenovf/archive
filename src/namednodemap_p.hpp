/**
 * @file namednodemap_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_DOM_NAMEDNODEMAP_P_HPP__
#define __CWT_DOM_NAMEDNODEMAP_P_HPP__

#include "../include/cwt/dom/namednodemap.hpp"
#include <pfs/map.hpp>
#include <pfs/atomic.hpp>

namespace cwt { namespace dom {

class node::impl;

class namednodemap::impl
{
public:
	typedef node::impl * item_type;
	typedef pfs::map<pfs::string, item_type> map_type;

	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;

    map_type _map;

    pfs::atomic_integer ref;
    map_type map;
    node::impl * parent;
    bool readonly;
    bool appendToParent;


public:
    impl(node::impl*);
    ~impl();

    node::impl* namedItem(const pfs::string& name) const;
    node::impl* namedItemNS(const pfs::string& nsURI, const pfs::string& localName) const;
    node::impl* setNamedItem(node::impl* arg);
    node::impl* setNamedItemNS(node::impl* arg);
    node::impl* removeNamedItem(const pfs::string& name);
    node::impl* item(int index) const;
    int length() const;
    bool contains(const pfs::string& name) const;
    bool containsNS(const pfs::string& nsURI, const pfs::string & localName) const;

    /**
     * Remove all children from the map.
     */
    void clearMap();
    bool isReadOnly() { return readonly; }
    void setReadOnly(bool r) { readonly = r; }
    bool isAppendToParent() { return appendToParent; }
    /**
     * If true, then the node will redirect insert/remove calls
     * to its parent by calling node::impl::appendChild or removeChild.
     * In addition the map wont increase or decrease the reference count
     * of the nodes it contains.
     *
     * By default this value is false and the map will handle reference counting
     * by itself.
     */
    void setAppendToParent(bool b) { appendToParent = b; }

    /**
     * Creates a copy of the map. It is a deep copy
     * that means that all children are cloned.
     */
    impl* clone(node::impl* parent);
};


}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_P_HPP__ */
