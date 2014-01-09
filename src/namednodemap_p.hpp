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

class node_impl;

class namednodemap_impl
{
public:
	typedef node_impl * item_type;
	typedef pfs::map<pfs::string, item_type> map_type;

	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;

    pfs::atomic_int _ref;
    map_type        _map;
    node_impl *     _parent;
    bool            _readonly;
    bool            _appendToParent;

public:
    namednodemap_impl (node_impl * nimpl)
    	: _ref(1)
    	, _parent(nimpl)
    	, _readonly(false)
    	, _appendToParent(false)
    {}

    ~namednodemap_impl () { clearMap(); }

    node_impl * namedItem (const pfs::string & name) const;
    node_impl * namedItemNS (const pfs::string & nsURI, const pfs::string & localName) const;
    node_impl * setNamedItem (node_impl * arg);
    node_impl * setNamedItemNS (node_impl * arg);
    node_impl * removeNamedItem (const pfs::string & name);
    node_impl * item (size_t index) const;
    size_t length () const { return _map.size(); }
    bool contains (const pfs::string & name) const { return _map.contains(name); }
    bool containsNS (const pfs::string & nsURI, const pfs::string & localName) const
    {
    	return namedItemNS(nsURI, localName) != nullptr;
    }
//
//    /**
//     * Remove all children from the map.
//     */
    void clearMap ();
//    bool isReadOnly () { return readonly; }
//    void setReadOnly (bool r) { readonly = r; }
//    bool isAppendToParent () { return appendToParent; }
//    /**
//     * If true, then the node will redirect insert/remove calls
//     * to its parent by calling node_impl::appendChild or removeChild.
//     * In addition the map wont increase or decrease the reference count
//     * of the nodes it contains.
//     *
//     * By default this value is false and the map will handle reference counting
//     * by itself.
//     */
    void setAppendToParent (bool b) { _appendToParent = b; }

    /**
     * Creates a deep copy (all children are cloned).
     */
    namednodemap_impl * clone (node_impl * parent);
};


}} // cwt::dom

#endif /* __CWT_DOM_NAMEDNODEMAP_P_HPP__ */
