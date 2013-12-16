/**
 * @file attr.cpp
 * @author wladt
 * @date Dec 12, 2013
 */

#include "node_p.hpp"
#include "attr_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

//
//pfs::string attr::name () const
//{
//	return node::nodeName ();
//}
//
//pfs::string attr::value () const
//{
//	return node::nodeValue ();
//}


#ifdef __COMMENT__

attr::impl::impl (pfs::shared_ptr<node::impl> & parent, const pfs::string & name)
	: node::impl(parent)
	, _specified(false)
{
	node::impl::_name = name;
}

attr::impl::impl (pfs::shared_ptr<node::impl> & parent, const pfs::string & namespaceURI, const pfs::string & qualifiedName)
	: node::impl(parent)
	, _specified(false)

{
	node::impl::_namespaceURI = namespaceURI;

	pfs::vector<pfs::string> parts = qualifiedName.split(pfs::ucchar(':'), true);
    if (parts.size() > 1) {
    	node::impl::_prefix = parts[0];
    	node::impl::_name = parts[1];
    } else {
    	node::impl::_name = qualifiedName;
    }
}

bool attr::specified () const
{
	return _pimpl
		? dynamic_pointer_cast<attr::impl>(_pimpl)->_specified
		: false;
}

void attr::setValue (const pfs::string & v)
{
    if (!_pimpl)
        return;
    _pimpl->setNodeValue(v);
    dynamic_pointer_cast<attr::impl>(_pimpl)->_specified = true;
}

element attr::ownerElement() const
{
    PFS_ASSERT(_pimpl->parent());
    if (!_pimpl->parent()->isElement())
        return element();
    return element(dynamic_pointer_cast<element::impl>(_pimpl->parent()));
}
#endif

}} // cwt::dom


