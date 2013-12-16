/**
 * @file chardata_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_CHARDATA_P_HPP__
#define __CWT_DOM_CHARDATA_P_HPP__

#include "../include/cwt/dom/chardata.hpp"

namespace cwt { namespace dom {

class node_impl;
class document_impl;

class chardata_impl : public node_impl
{
public:
	chardata_impl (document_impl *, node_impl * parent, const pfs::string & data);
	chardata_impl (chardata_impl * n, bool deep);

    int dataLength () const;
    pfs::string substringData (unsigned long offset, unsigned long count) const;
    void appendData  (const pfs::string & arg);
    void insertData  (unsigned long offset, const pfs::string & arg);
    void deleteData  (unsigned long offset, unsigned long count);
    void replaceData (unsigned long offset, unsigned long count, const pfs::string & arg);

    // Reimplemented from QDomNodePrivate
    node::type nodeType () const { return node::CharacterDataNode; }
    node_impl * cloneNode (bool deep = true);
};

}} // cwt::dom

#endif /* __CWT_DOM_CHARDATA_P_HPP__ */
