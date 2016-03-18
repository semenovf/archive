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

namespace pfs { namespace dom {

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

size_t chardata_impl::data_length () const
{
    return _value.length();
}

pfs::string chardata_impl::substring_data (size_t offset, size_t n) const
{
    return _value.mid(offset, n);
}

void chardata_impl::append_data(const pfs::string & arg)
{
    _value.append(arg);
}

void chardata_impl::insert_data (size_t offset, const pfs::string & arg)
{
    _value.insert(offset, arg);
}

void chardata_impl::delete_data (size_t offset, size_t n)
{
    _value.erase(offset, n);
}

void chardata_impl::replace_data (size_t offset, size_t n, const pfs::string & arg)
{
    _value.replace(offset, n, arg);
}


node_impl * chardata_impl::clone_node (bool deep)
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
	return _d
			? _d->node_value()
			: pfs::string();
}

void chardata::set_data (const pfs::string & v)
{
    if (_d)
        _d->set_node_value(v);
}

size_t chardata::length () const
{
	return _d
			? dynamic_cast<chardata_impl*>(_d)->data_length()
			: 0;
}

pfs::string chardata::substring_data (size_t offset, size_t n) const
{
	return _d
			? dynamic_cast<chardata_impl*>(_d)->substring_data(offset, n)
			: pfs::string();
}

void chardata::append_data (const pfs::string & arg)
{
    if (_d)
    	dynamic_cast<chardata_impl*>(_d)->append_data(arg);
}

void chardata::insert_data (size_t offset, const pfs::string & arg)
{
    if (_d)
    	dynamic_cast<chardata_impl*>(_d)->insert_data(offset, arg);
}

void chardata::delete_data (size_t offset, size_t count)
{
    if (_d)
    	dynamic_cast<chardata_impl*>(_d)->delete_data(offset, count);
}

void chardata::replace_data(size_t offset, size_t count, const pfs::string & arg)
{
    if (_d)
    	dynamic_cast<chardata_impl*>(_d)->replace_data(offset, count, arg);
}

node::type chardata::node_type() const
{
	return _d
			? node::node_type()
			: invalid_node;
}

}} // pfs::dom
