/**
 * @file domimpl.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "doctype_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "document_p.hpp"
#include "domimpl_p.hpp"

namespace pfs { namespace dom {

dom_implementation_impl * dom_implementation_impl::clone()
{
    return new dom_implementation_impl;
}

bool dom_implementation_impl::hasFeature (const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    return false;
}


dom_implementation::dom_implementation()
	: _pimpl(new dom_implementation_impl)
{
	_pimpl->ref.ref();
}

dom_implementation::dom_implementation (const dom_implementation & other)
	: _pimpl(other._pimpl)
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

dom_implementation & dom_implementation::operator = (const dom_implementation & other)
{
    if (other._pimpl)
        other._pimpl->ref.ref();
    if (_pimpl && !_pimpl->ref.deref())
        delete _pimpl;
    _pimpl = other._pimpl;
    return *this;
}

dom_implementation::~dom_implementation()
{
    if (_pimpl && !_pimpl->ref.deref())
        delete _pimpl;
}

bool dom_implementation::hasFeature (const pfs::string & feature, const pfs::string & version) const
{
    return _pimpl
    			? _pimpl->hasFeature(feature, version)
    			: false;
}

document dom_implementation::createDocument(const pfs::string & nsURI
		, const pfs::string & qName
		, const doctype & doctype)
{
    document doc(doctype);
    element root = doc.createElementNS(nsURI, qName);

    if (root.isNull())
        return document();

    doc.appendChild(root);
    return doc;
}

doctype dom_implementation::createDocumentType (const pfs::string & qName
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

    doctype_impl * dt = new doctype_impl(0);
    dt->_name = qName; //fixedName;
    if (systemId.isNull()) {
        dt->_publicId.clear();
        dt->_systemId.clear();
    } else {
        dt->_publicId = publicId; // fixedPublicId;
        dt->_systemId = systemId; // fixedSystemId;
    }
    dt->ref.deref();
    return doctype(dt);
}

pfs::string dom_implementation::idName () const
{
	return _pimpl ? _pimpl->idName() : pfs::string();
}

}} // pfs::dom



