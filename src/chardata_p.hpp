/**
 * @file chardata_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __CWT_DOM_CHARDATA_P_HPP__
#define __CWT_DOM_CHARDATA_P_HPP__

#include "../include/cwt/dom/chardata.hpp"

namespace cwt { namespace dom {

class node::impl;
class document::impl;

class chardata::impl : public node::impl
{
public:
    impl(document::impl*, QDomNodePrivate* parent, const pfs::string& data);
    impl(impl* n, bool deep);

    int dataLength() const;
    pfs::string substringData(unsigned long offset, unsigned long count) const;
    void appendData(const pfs::string& arg);
    void insertData(unsigned long offset, const pfs::string& arg);
    void deleteData(unsigned long offset, unsigned long count);
    void replaceData(unsigned long offset, unsigned long count, const pfs::string& arg);

    // Reimplemented from QDomNodePrivate
    node::type nodeType() const { return node::CharacterDataNode; }
    node::impl * cloneNode(bool deep = true);
};



class document::impl;

class chardata::impl : public node::impl
{
public:
    impl (document::impl *, node::impl* parent = 0);
    impl (node::impl* n, bool deep);

    // Reimplemented from node::impl
    virtual node::impl* cloneNode(bool deep = true);
    node::type nodeType() const { return node::DocumentFragmentNode; }
};

}} // cwt::dom

#endif /* __CWT_DOM_CHARDATA_P_HPP__ */
