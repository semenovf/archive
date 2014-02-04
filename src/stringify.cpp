/**
 * @file stringify.cpp
 * @author wladt
 * @date Feb 3, 2014
 */

#include "dom_p.hpp"

namespace cwt { namespace xml {

static void __stringify_node                  (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_null                  (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_element               (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_attribute             (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_text                  (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_CDATASection          (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_entityReference       (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_entity                (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_processingInstruction (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_comment               (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_notation              (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_documentType          (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
static void __stringify_document              (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);

struct _stringify_vtable
{
	void (* to_string) (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent);
} stringify_vtable[] = {
	  { __stringify_null }
	, { __stringify_element }
	, { __stringify_attribute }
	, { __stringify_text }
	, { __stringify_CDATASection }
	, { __stringify_entityReference }
	, { __stringify_entity }
	, { __stringify_processingInstruction }
	, { __stringify_comment }
	, { __stringify_document }
	, { __stringify_documentType }
	, { __stringify_notation }
};

/*
inline pfs::string __indent (int count, char ch)
{
	return count > 0
			? pfs::string(1, '\n') + pfs::string(count, ch)
			: pfs::string();
}
*/

static pfs::string __quotedValue (const pfs::string & data)
{
    pfs::ucchar quote = data.find(pfs::string(1, '\'')) == data.cend()
                    ? pfs::ucchar('\'')
                    : pfs::ucchar('"');
    pfs::string r;
    r << quote << data << quote;
    return r;
}

/**
 * @internal
 *
 * @param str
 * @param encodeQuotes
 * @param performAVN
 * @param encodeEOLs
 * @return
 *
 * @note see http://www.w3.org/TR/xml/
 * 		2.11 End-of-Line Handling
 * 		3.3.3 Attribute-Value Normalization
 */
static pfs::string __encodeText (const pfs::string & str
		, const bool encodeQuotes = true
		, const bool performAVN = false
		, const bool encodeEOLs = false)
{
    pfs::string r;

    pfs::string::const_iterator it = str.cbegin();
    pfs::string::const_iterator itEnd = str.cend();

 //   int closeBracket = 0;

    for (; it != itEnd; ++it) {
        if (*it == pfs::ucchar('<')) {
            r << "&lt;";
        } else if (encodeQuotes && *it == pfs::ucchar('"')) {
            r << "&quot;";
        } else if (*it == pfs::ucchar('&')) {
            r << "&amp;";
        } else if (*it == pfs::ucchar('>')
        		&& it + 2 < itEnd
        		&& *(it + 1) == pfs::ucchar(']')
        		&& *(it + 2) == pfs::ucchar(']')) {
            r << "&gt;";
        } else if (performAVN &&
                   (*it == pfs::ucchar(0xA) ||
                    *it == pfs::ucchar(0xD) ||
                    *it == pfs::ucchar(0x9))) {
        	pfs::ucchar ch(*it);
        	r << "&#x" << pfs::string::number(ch.unicode()) << ';';
        } else if (encodeEOLs && *it == pfs::ucchar(0xD)) {
            r << "&#xd;"; // Replace a single 0xD with a ref for 0xD
        } else {
			// We have to use a character reference to get it through.
/*
			const ushort codepoint(ati.unicode());
			const QString replacement(QLatin1String("&#x") + QString::number(codepoint, 16) + QLatin1Char(';'));
			retval.replace(i, 1, replacement);
			i += replacement.length();
			len += replacement.length() - 1;
*/
			r << *it;
        }
    }

    return r;
}

static pfs::string __encodeEntity (const pfs::string & str)
{
    pfs::string r;

    pfs::string::const_iterator it = str.cbegin();
    pfs::string::const_iterator itEnd = str.cend();

    for (; it != itEnd; ++it) {
        if (*it == pfs::ucchar('%')){
        	r << "&#60;";
        }  else if (*it == pfs::ucchar('"')) {
        	r << "&#34;";
        } else if (*it == pfs::ucchar('&') && it + 1 < itEnd && *(it + 1) == pfs::ucchar('#')) {
            // Don't encode &lt; or &quot; or &custom;.
            // Only encode character references
            r << "&#38;";
        } else {
            r << pfs::string(1, *it);
        }
    }

    return r;
}

void __stringify_node (const cwt::dom::node_impl & node, pfs::string & s, int depth, int indent)
{
    const cwt::dom::node_impl * n = node._first;
    while (n) {
    	stringify_vtable[n->nodeType()].to_string(*n, s, depth, indent);
        n = n->_next;
    }
}

void __stringify_null (const cwt::dom::node_impl &, pfs::string &, int , int )
{}


void __stringify_element (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent)
{
    if (!(n._prev && n._prev->isText()))
        s << pfs::string(indent < 1 ? 0 : depth * indent, ' ');

    pfs::string qName(n._name);
    pfs::string nsDecl;

    if (!n._namespaceURI.isEmpty()) {

        /*
         * If we still have QDom, optimize this so that we only declare namespaces that are not
         * yet declared. We loose default namespace mappings, so maybe we should rather store
         * the information that we get from startPrefixMapping()/endPrefixMapping() and use them.
         * Modifications becomes more complex then, however.
         *
         * We cannot do this in a patch release because it would require too invasive changes, and
         * hence possibly behavioral changes.
         */
        if (n._prefix.isEmpty()) {
            nsDecl << " xmlns";
        } else {
        	qName.clear();
            qName << n._prefix << pfs::string(1, ':') << n._name;
            nsDecl << " xmlns:" << n._prefix;
        }
        nsDecl << "=\"" << __encodeText(n._namespaceURI) << "\"";
    }
    s << "<" << qName << nsDecl;

    pfs::map<pfs::string, bool> outputtedPrefixes;
    const cwt::dom::element_impl & n1 = dynamic_cast<const cwt::dom::element_impl &>(n);

    /* Write out attributes. */
    if (!n1._attr->_map.isEmpty()) {
        pfs::map<pfs::string, cwt::dom::node_impl *>::const_iterator it = n1._attr->_map.cbegin();

        for (; it != n1._attr->_map.cend(); ++it) {
            s << " ";
            if (it->second->_namespaceURI.isEmpty()) {
                s << it->second->_name << "=\"" << __encodeText(it->second->_value, true, true) << "\"";
            } else {
                s << it->second->_prefix << ':' << it->second->_name << "=\"" << __encodeText(it->second->_value, true, true) << "\"";
                /* This is a fix for 138243, as good as it gets.
                 *
                 * QDomElementPrivate::save() output a namespace declaration if
                 * the element is in a namespace, no matter what. This function do as well, meaning
                 * that we get two identical namespace declaration if we don't have the if-
                 * statement below.
                 *
                 * This doesn't work when the parent element has the same prefix as us but
                 * a different namespace. However, this can only occur by the user modifying the element,
                 * and we don't do fixups by that anyway, and hence it's the user responsibility to not
                 * arrive in those situations. */
                if ((!it->second->_ownerNode
                		|| it->second->_ownerNode->_prefix != it->second->_prefix)
                		&& !outputtedPrefixes.contains(it->second->_prefix)) {

                    s << " xmlns:" << it->second->_prefix << "=\"" << __encodeText(it->second->_namespaceURI, true, true) << "\"";
                    outputtedPrefixes.insert(it->second->_prefix, true);
                }
            }
        }
    }

    if (n._last) {
        // has child nodes
        if (n._first->isText())
            s << ">";
        else {
            s << ">";

            /* -1 disables new lines. */
            if (indent != -1)
                s << "\n";
        }

        __stringify_node(n, s, depth + 1, indent);

        if (!n._last->isText())
            s << pfs::string(indent < 1 ? 0 : depth * indent, ' ');

        s << "</" << qName << ">";
    } else {
        s << "/>";
    }
    if (!(n._next && n._next->isText())) {
        /* -1 disables new lines. */
        if (indent != -1)
            s << "\n";
    }
}

void __stringify_attribute (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
    if (n._namespaceURI.isEmpty()) {
        s << n._name << "=\"" << __encodeText(n._value, true, true) << '\"';
    } else {
        s << n._prefix << ':' << n._name << "=\"" << __encodeText(n._value, true, true) << '\"';
        /* This is a fix for 138243, as good as it gets.
         *
         * QDomElementPrivate::save() output a namespace declaration if
         * the element is in a namespace, no matter what. This function do as well, meaning
         * that we get two identical namespace declaration if we don't have the if-
         * statement below.
         *
         * This doesn't work when the parent element has the same prefix as us but
         * a different namespace. However, this can only occur by the user modifying the element,
         * and we don't do fixups by that anyway, and hence it's the user responsibility to not
         * arrive in those situations. */
        if (!n._ownerNode ||
           n._ownerNode->_prefix != n._prefix) {
            s << " xmlns:" << n._prefix << "=\"" << __encodeText(n._namespaceURI, true, true) << '\"';
        }
    }
}

void __stringify_text (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
    const cwt::dom::text_impl & n1 = dynamic_cast<const cwt::dom::text_impl &>(n);
    s << __encodeText(n1._value, !(n1.parent() && n1.parent()->isElement()), false, true);
}

void __stringify_CDATASection (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
    s << "<![CDATA[" << n._value << "]]>";
}

void __stringify_entityReference (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
	s << "&" << n._name << ";";
}

void __stringify_entity (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
	const cwt::dom::entity_impl & n1 = dynamic_cast<const cwt::dom::entity_impl &>(n);

    pfs::string _name = n._name;

    if (_name.startsWith("%"))
        _name = pfs::string("% ") + _name.substr(1);

    if (n1._sys.isEmpty() && n1._pub.isEmpty()) {
        s << "<!ENTITY " << _name << " \"" << __encodeEntity(n._value) << "\">" << "\n";
    } else {
        s << "<!ENTITY " << _name << " ";
        if (n1._pub.isEmpty()) {
            s << "SYSTEM " << __quotedValue(n1._sys);
        } else {
            s << "PUBLIC " << __quotedValue(n1._pub) << " " << __quotedValue(n1._sys);
        }
        if (! n1._notationName.isEmpty()) {
            s << " NDATA " << n1._notationName;
        }
        s << ">" << "\n";
    }
}

void __stringify_processingInstruction (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
	 s << "<?" << n._name << ' ' << n._value << "?>" << "\n";
}

void __stringify_comment (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent)
{
	   /* We don't output whitespace if we would pollute a text node. */
	    if (!(n._prev && n._prev->isText()))
	        s << pfs::string(indent < 1 ? 0 : depth * indent, ' ');

	    s << "<!--" << n._value;
	    if (n._value.endsWith("-"))
	        s << ' '; // Ensures that XML comment doesn't end with --->
	    s << "-->";

	    if (!(n._next && n._next->isText()))
	        s << "\n";
}

void __stringify_notation (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int /*indent*/)
{
	const cwt::dom::notation_impl & n1 = dynamic_cast<const cwt::dom::notation_impl &>(n);

    s << "<!NOTATION " << n._name << ' ';

    if (!n1._pub.isEmpty())  {
        s << "PUBLIC " << __quotedValue(n1._pub);

        if (!n1._sys.isEmpty())
            s << ' ' << __quotedValue(n1._sys);
    }  else {
        s << "SYSTEM " << __quotedValue(n1._sys);
    }
    s << '>' << '\n';
}

void __stringify_documentType (const cwt::dom::node_impl & n, pfs::string & s, int /*depth*/, int indent)
{
	if (n._name.isEmpty())
		return;

	const cwt::dom::doctype_impl & n1 = dynamic_cast<const cwt::dom::doctype_impl &>(n);

	s << "<!DOCTYPE " << n1._name;

	if (!n1._publicId.isEmpty()) {
		s << " PUBLIC " << __quotedValue(n1._publicId);
		if (!n1._systemId.isEmpty()) {
			s << ' ' << __quotedValue(n1._systemId);
		}
	} else if (!n1._systemId.isEmpty()) {
		s << " SYSTEM " << __quotedValue(n1._systemId);
	}

	if (n1._entities->length() > 0 || n1._notations->length() > 0) {
		s << " [\n";

		pfs::map<pfs::string, cwt::dom::node_impl *>::const_iterator it2 = n1._notations->_map.cbegin();
		for (; it2 != n1._notations->_map.cend(); ++it2)
			stringify_vtable[it2->second->nodeType()].to_string(*it2->second, s, 0, indent);

		pfs::map<pfs::string, cwt::dom::node_impl *>::const_iterator it = n1._entities->_map.cbegin();
		for (; it != n1._entities->_map.cend(); ++it)
			stringify_vtable[it->second->nodeType()].to_string(*it->second, s, 0, indent);

		s << "]";
	}

	s << ">\n";
}

void __stringify_document (const cwt::dom::node_impl & n, pfs::string & s, int depth, int indent)
{
    const cwt::dom::node_impl * n1 = n._first;

    s << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << '\n';

    const cwt::dom::doctype_impl * dt = dynamic_cast<const cwt::dom::document_impl *>(& n)->doctype();
    if (dt) {
    	__stringify_documentType (*dt, s, depth, indent);
    }

    //  Skip the first processing instruction by name "xml", if any such exists.
    const cwt::dom::node_impl * startNode = n1;

	// First, we try to find the PI and sets the startNode to the one appearing after it.
	while (n1) {
		if(n1->isProcessingInstruction() && n1->nodeName() == "xml") {
			startNode = n1->_next;
			break;
		} else {
			n1 = n1->_next;
		}
	}

	// Now we serialize all the nodes after the faked XML declaration(the PI).
	while (startNode) {
		stringify_vtable[startNode->nodeType()].to_string(*startNode, s, 0, indent);
		startNode = startNode->_next;
	}
}

pfs::string dom::toString (const cwt::dom::document & doc) const
{
	pfs::string r;
	if (doc.pimpl()) {
		__stringify_document(*doc.pimpl(), r, 0, 4);
	}
	return r;
}

}} // cwt::xml
