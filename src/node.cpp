#include "../include/cwt/dom/node.hpp"
#include "../include/cwt/dom/nodelist.hpp"


namespace cwt { namespace dom {

node::node()
	: _nodeName()
	, _nodeValue()
	, _nodeType()
    , _parentNode(nullptr)
    , _childNodes()
    , _firstChild(nullptr)
    , _lastChild(nullptr)
    , _previousSibling(nullptr)
    , _nextSibling(nullptr)
    , _attributes()
	, _ownerDocument(nullptr)
	, _namespaceURI()
	, _prefix()
	, _localName()
{}


}}
