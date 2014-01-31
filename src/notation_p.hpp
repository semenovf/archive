/**
 * @file notation_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_NOTATION_P_HPP__
#define __CWT_DOM_NOTATION_P_HPP__

#include "../include/cwt/dom/notation.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class DLL_API notation_impl : public node_impl
{
public:
    pfs::string _sys;
    pfs::string _pub;

public:
    notation_impl (document_impl *, node_impl * parent, const pfs::string & name,
                          const pfs::string & pub, const pfs::string & sys);
    notation_impl (notation_impl * n, bool deep);

    virtual node_impl * cloneNode (bool deep = true);
    virtual node::type nodeType() const { return node::NotationNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_NOTATION_P_HPP__ */
