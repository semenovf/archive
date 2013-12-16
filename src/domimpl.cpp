/**
 * @file domimpl.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "domimpl_p.hpp"
#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "document_p.hpp"
#include "doctype_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"

namespace cwt { namespace dom {

dom_implementation_impl * dom_implementation_impl::clone()
{
    return new dom_implementation_impl;
}

bool dom_implementation::hasFeature (const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    return false;
}


dom_implementation::dom_implementation()
	: _pimpl(nullptr)
{}


dom_implementation::dom_implementation (const dom_implementation & x)
	: _pimpl(x._pimpl)
{
    if (_pimpl)
        _pimpl->ref.ref();
}

dom_implementation::dom_implementation (dom_implementation_impl * p)
	: _pimpl(p)
{
    if (_pimpl)
        _pimpl->ref.ref();
}

/*
dom_implementation& dom_implementation::operator=(const dom_implementation &x)
{
    if (x.impl)
        x.impl->ref.ref();
    if (impl && !impl->ref.deref())
        delete impl;
    impl = x.impl;
    return *this;
}
*/

/*
bool dom_implementation::operator==(const dom_implementation &x) const
{
    return (impl == x.impl);
}
*/

/*
bool dom_implementation::operator!=(const dom_implementation &x) const
{
    return (impl != x.impl);
}
*/

dom_implementation::~dom_implementation()
{
    if (_pimpl && !_pimpl->ref.deref())
        delete _pimpl;
}

document_type dom_implementation::createDocumentType (const pfs::string & qName
		, const pfs::string & publicId
		, const pfs::string & systemId)
{
/*
    bool ok;
    pfs::string fixedName = fixedXmlName(qName, &ok, true);
    if (!ok)
        return document_type();

    pfs::string fixedPublicId = fixedPubidLiteral(publicId, &ok);
    if (!ok)
        return document_type();

    pfs::string fixedSystemId = fixedSystemLiteral(systemId, &ok);
    if (!ok)
        return document_type();
*/

    document_type_impl * dt = new document_type_impl(nullptr);
    dt->name = qName; //fixedName;
    if (systemId.isNull()) {
        dt->_publicId.clear();
        dt->_systemId.clear();
    } else {
        dt->_publicId = publicId; // fixedPublicId;
        dt->_systemId = systemId; // fixedSystemId;
    }
    dt->ref.deref();
    return document_type(dt);
}

/*!
    Creates a DOM document with the document type \a doctype. This
    function also adds a root element node with the qualified name \a
    qName and the namespace URI \a nsURI.
*/
document dom_implementation::createDocument(const pfs::string & nsURI
		, const pfs::string & qName
		, const document_type & doctype)
{
    document doc(doctype);
    element root = doc.createElementNS(nsURI, qName);

    if (root.isNull())
        return document();

    doc.appendChild(root);
    return doc;
}

//dom_implementation::InvalidDataPolicy dom_implementation::invalidDataPolicy()
//{
//    return dom_implementation_impl::invalidDataPolicy;
//}
//
//void dom_implementation::setInvalidDataPolicy(InvalidDataPolicy policy)
//{
//    dom_implementation_impl::invalidDataPolicy = policy;
//}

}} // cwt::dom



