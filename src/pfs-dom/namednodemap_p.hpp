/**
 * @file namednodemap_p.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __PFS_DOM_NAMEDNODEMAP_P_HPP__
#define __PFS_DOM_NAMEDNODEMAP_P_HPP__

#include <pfs/map.hpp>
#include <pfs/atomic.hpp>
#include "pfs/dom/namednodemap.hpp"

namespace pfs { namespace dom {

class node_impl;

class DLL_API namednodemap_impl
{
public:
	typedef node_impl * item_type;
	typedef map<string, item_type> map_type;

	typedef map_type::iterator iterator;
	typedef map_type::const_iterator const_iterator;

    pfs::atomic_int _ref;
    map_type        _map;
    node_impl *     _parent;
    bool            _readonly;
    bool            _append_to_parent;

public:
    namednodemap_impl (node_impl * nimpl)
    	: _ref(1)
    	, _parent(nimpl)
    	, _readonly(false)
    	, _append_to_parent(false)
    {}

    ~namednodemap_impl () { clear_map(); }

    node_impl * named_item (const string & name) const;
    node_impl * named_item_ns (const string & ns_uri, const pfs::string & localname) const;
    node_impl * set_named_item (node_impl * arg);
    node_impl * set_named_item_ns (node_impl * arg);
    node_impl * remove_named_item (const string & name);
    node_impl * item (size_t index) const;

    size_t length () const
    {
    	return _map.size();
    }

    bool contains (const string & name) const
    {
    	// return _map.contains(name);
    	return _map.find(name) != _map.end();
    }

    bool contains_ns (const string & ns_uri, const string & localName) const
    {
    	return named_item_ns(ns_uri, localName) != 0;
    }

    /*
     * Remove all children from the map.
     */
    void clear_map ();

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
    void set_append_to_parent (bool b)
    {
    	_append_to_parent = b;
    }

    /*
     * Creates a deep copy (all children are cloned).
     */
    namednodemap_impl * clone (node_impl * parent);
};


}} // pfs::dom

#endif /* __PFS_DOM_NAMEDNODEMAP_P_HPP__ */
