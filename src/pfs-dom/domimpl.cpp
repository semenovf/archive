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

bool dom_implementation_impl::has_feature (const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    return false;
}


dom_implementation::dom_implementation()
	: _d(new dom_implementation_impl)
{
	_d->ref.ref();
}

dom_implementation::dom_implementation (const dom_implementation & other)
	: _d(other._d)
{
    if (_d)
        _d->ref.ref();
}


dom_implementation::dom_implementation (dom_implementation_impl * p)
	: _d(p)
{
    if (_d)
        _d->ref.ref();
}

dom_implementation & dom_implementation::operator = (const dom_implementation & other)
{
    if (other._d)
        other._d->ref.ref();
    if (_d && !_d->ref.deref())
        delete _d;
    _d = other._d;
    return *this;
}

dom_implementation::~dom_implementation()
{
    if (_d && !_d->ref.deref())
        delete _d;
}

bool dom_implementation::has_feature (const pfs::string & feature, const pfs::string & version) const
{
    return _d
    			? _d->has_feature(feature, version)
    			: false;
}

document dom_implementation::create_document(const pfs::string & nsURI
		, const pfs::string & qName
		, const doctype & doctype)
{
    document doc(doctype);
    element root = doc.create_element_ns(nsURI, qName);

    if (root.is_null())
        return document();

    doc.append_child(root);
    return doc;
}

doctype dom_implementation::create_document_type (const pfs::string & qName
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
    if (systemId.is_null()) {
        dt->_public_id.clear();
        dt->_system_id.clear();
    } else {
        dt->_public_id = publicId; // fixedPublicId;
        dt->_system_id = systemId; // fixedSystemId;
    }
    dt->ref.deref();
    return doctype(dt);
}

pfs::string dom_implementation::idname () const
{
	return _d ? _d->idname() : pfs::string();
}

}} // pfs::dom



