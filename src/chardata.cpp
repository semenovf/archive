/**
 * @file chardata.cpp
 * @author wladt
 * @date Jan 9, 2014
 */

#include "node_p.hpp"
#include "nodelist_p.hpp"
#include "namednodemap_p.hpp"
#include "attr_p.hpp"
#include "element_p.hpp"
#include "chardata_p.hpp"
#include "text_p.hpp"
#include "cdatasection_p.hpp"
#include "document_p.hpp"

namespace cwt { namespace dom {

chardata_impl::chardata_impl (document_impl * d, node_impl * p, const pfs::string & data)
    : node_impl(d, p)
{
	_value = data;
	_name = pfs::string("#character-data");
}

chardata_impl::chardata_impl (chardata_impl* n, bool deep)
    : node_impl(n, deep)
{
}

size_t chardata_impl::dataLength () const
{
    return _value.length();
}

pfs::string chardata_impl::substringData (size_t offset, size_t n) const
{
    return _value.mid(offset, n);
}

void chardata_impl::appendData(const pfs::string & arg)
{
    _value += arg;
}

void chardata_impl::insertData (size_t offset, const pfs::string & arg)
{
    _value.insert(offset, arg);
}

void chardata_impl::deleteData (size_t offset, size_t n)
{
    _value.remove(offset, n);
}

void chardata_impl::replaceData (size_t offset, size_t n, const pfs::string & arg)
{
    _value.replace(offset, n, arg);
}


node_impl * chardata_impl::cloneNode (bool deep)
{
    node_impl * p = new chardata_impl(this, deep);
    p->ref.deref();
    return p;
}

chardata::chardata()
{
}

chardata::chardata (const chardata & other)
    : node(other)
{
}

chardata::chardata (chardata_impl * n)
    : node(n)
{
}

chardata & chardata::operator = (const chardata & other)
{
    return (chardata &) node::operator = (other);
}

pfs::string chardata::data () const
{
	return _pimpl
			? _pimpl->nodeValue()
			: pfs::string();
}

void chardata::setData (const pfs::string & v)
{
    if (_pimpl)
        _pimpl->setNodeValue(v);
}

size_t chardata::length () const
{
	return _pimpl
			? dynamic_cast<chardata_impl*>(_pimpl)->dataLength()
			: 0;
}

pfs::string chardata::substringData (size_t offset, size_t n) const
{
	return _pimpl
			? dynamic_cast<chardata_impl*>(_pimpl)->substringData(offset, n)
			: pfs::string();
}

void chardata::appendData (const pfs::string & arg)
{
    if (_pimpl)
    	dynamic_cast<chardata_impl*>(_pimpl)->appendData(arg);
}

void chardata::insertData (size_t offset, const pfs::string & arg)
{
    if (_pimpl)
    	dynamic_cast<chardata_impl*>(_pimpl)->insertData(offset, arg);
}

void chardata::deleteData (size_t offset, size_t count)
{
    if (_pimpl)
    	dynamic_cast<chardata_impl*>(_pimpl)->deleteData(offset, count);
}

void chardata::replaceData(size_t offset, size_t count, const pfs::string & arg)
{
    if (_pimpl)
    	dynamic_cast<chardata_impl*>(_pimpl)->replaceData(offset, count, arg);
}

node::type chardata::nodeType() const
{
	return _pimpl
			? node::nodeType()
			: InvalidNode;
}

}} // cwt::dom
