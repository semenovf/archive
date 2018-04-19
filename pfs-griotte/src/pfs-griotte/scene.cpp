/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <pfs.hpp>
#include "pfs/griotte/scene.hpp"
#include "pfs/griotte/widget.hpp"

namespace pfs {
namespace griotte {

node::~node ()
{
    node_iterator it = children.begin();
    node_iterator it_end = children.end();
    
    for (; it != it_end; ++it) {
        PFS_ASSERT(it->pwidget)
        delete it->pwidget;
    }
}

void scene::delete_widget (widget * w)
{
    if (w) {
        PFS_ASSERT(w->_pnode);
        node * parent_node = w->_pnode->parent;
        PFS_ASSERT(parent_node);
        
        node::node_iterator it = parent_node->children.begin();
        node::node_iterator it_end = parent_node->children.end();

        for (; it != it_end; ++it) {
            if (w == it->pwidget) {
                it->~node();
                parent_node->children.erase(it);
                break;
            }
        }
    }
}

void scene::add_widget (widget * w, widget * parent_widget)
{
    node * parent_node = parent_widget 
            ? parent_widget->_pnode
            : & _root;
    
    PFS_ASSERT(parent_node);
    parent_node->children.push_back(node());
    node & n = parent_node->children.back();
    
    n.parent = parent_node;
    n.pwidget = w;
    
    w->_pnode = & n;
}

}}