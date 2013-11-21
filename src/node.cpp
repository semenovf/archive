#include "../include/cwt/dom/node.hpp"

namespace cwt { namespace dom {

node::node()
	: _nodeName()
	, _nodeValue()
	, _nodeType()
    , _parentNode(nullptr)
    , _childNodes(nullptr)
    , _firstChild(nullptr)
    , _lastChild(nullptr)
    , _previousSibling(nullptr)
    , _nextSibling(nullptr)
    , _attributes(nullptr)
{}

}}
