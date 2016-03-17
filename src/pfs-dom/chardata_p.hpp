/**
 * @file chardata_p.hpp
 * @author wladt
 * @date Dec 13, 2013
 */

#ifndef __PFS_DOM_CHARDATA_P_HPP__
#define __PFS_DOM_CHARDATA_P_HPP__

#include "pfs/dom/chardata.hpp"

namespace pfs { namespace dom {

class node_impl;
class document_impl;

class DLL_API chardata_impl : public node_impl
{
public:
	chardata_impl (document_impl *, node_impl * parent, const pfs::string & data);
	chardata_impl (chardata_impl * n, bool deep);

    size_t data_length () const;
    pfs::string substring_data (size_t offset, size_t count) const;
    void append_data  (const pfs::string & arg);
    void insert_data  (size_t offset, const pfs::string & arg);
    void delete_data  (size_t offset, size_t count);
    void replace_data (size_t offset, size_t count, const pfs::string & arg);

    virtual node::type node_type () const { return node::invalid_node; }
    virtual node_impl * clone_node (bool deep = true);
};

}} // pfs::dom

#endif /* __PFS_DOM_CHARDATA_P_HPP__ */
