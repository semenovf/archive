/**
 * @file domimpl.cpp
 * @author wladt
 * @date Dec 11, 2013
 */

#include "domimpl_p.hpp"
#include "document_p.hpp"
#include "doctype_p.hpp"

namespace cwt { namespace dom {

bool dom_implementation::hasFeature (const pfs::string & feature, const pfs::string & version) const
{
    if (feature == pfs::string("XML")) {
        if (version.isEmpty() || version == pfs::string("1.0")) {
            return true;
        }
    }
    return false;
}


/*!
    Constructs a dom_implementation object.
*/
dom_implementation::dom_implementation()
	: _pimpl(nullptr)
{}

/*!
    Constructs a copy of \a x.
*/
dom_implementation::dom_implementation(const dom_implementation & x)
	: _pimpl(x._pimpl)
{
    if (_pimpl)
        _pimpl->ref.ref();
}

dom_implementation::dom_implementation(dom_implementation::impl * p)
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
        delete impl;
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

    document_type::impl *dt = new document_type::impl(0);
    dt->name = qName; //fixedName;
    if (systemId.isNull()) {
        dt->publicId.clear();
        dt->systemId.clear();
    } else {
        dt->publicId = publicId; // fixedPublicId;
        dt->systemId = systemId; // fixedSystemId;
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

/*!
    Returns false if the object was created by
    document::implementation(); otherwise returns true.
*/
bool dom_implementation::isNull()
{
    return (impl == 0);
}

/*!
    \enum dom_implementation::InvalidDataPolicy

    This enum specifies what should be done when a factory function
    in document is called with invalid data.
    \value AcceptInvalidChars The data should be stored in the DOM object
        anyway. In this case the resulting XML document might not be well-formed.
        This is the default value and QDom's behavior in Qt < 4.1.
    \value DropInvalidChars The invalid characters should be removed from
        the data.
    \value ReturnNullNode The factory function should return a null node.

    \sa setInvalidDataPolicy(), invalidDataPolicy()
*/

/*!
   \enum node::EncodingPolicy
   \since 4.3

   This enum specifies how node::save() determines what encoding to use
   when serializing.

   \value EncodingFromDocument The encoding is fetched from the document.
   \value EncodingFromTextStream The encoding is fetched from the QTextStream.

   \sa node::save()
*/

/*!
    \since 4.1
    \nonreentrant

    Returns the invalid data policy, which specifies what should be done when
    a factory function in document is passed invalid data.

    \sa setInvalidDataPolicy(), InvalidDataPolicy
*/

dom_implementation::InvalidDataPolicy dom_implementation::invalidDataPolicy()
{
    return dom_implementation::impl::invalidDataPolicy;
}

/*!
    \since 4.1
    \nonreentrant

    Sets the invalid data policy, which specifies what should be done when
    a factory function in document is passed invalid data.

    The \a policy is set for all instances of document which already
    exist and which will be created in the future.

    \snippet code/src_xml_dom_qdom.cpp 0

    \sa invalidDataPolicy(), InvalidDataPolicy
*/

void dom_implementation::setInvalidDataPolicy(InvalidDataPolicy policy)
{
    dom_implementation::impl::invalidDataPolicy = policy;
}

}} // cwt::dom



