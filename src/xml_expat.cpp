/**
 * @file xml_expat.cpp
 * @author wladt
 * @date Dec 9, 2013
 */
#include "xml_expat.hpp"

namespace cwt { namespace xml {

const XML_Char reader::impl::NamespaceSeparator(0xFF);

PFS_PIMPL_DEF(reader, impl);

static bool __accept_version (int, int)
{
	return true;
}

reader::impl::impl ()
	: _parser(nullptr)
	, _handlers(nullptr)
	, _acceptVersion(__accept_version)
{
	_parser = XML_ParserCreateNS("UTF-8", NamespaceSeparator);
	PFS_ASSERT(_parser);

    XML_SetStartElementHandler(_parser, xml_startElementHandler);
    XML_SetEndElementHandler(_parser, xml_endElementHandler);
//    XML_SetElementHandler(_parser); // equivalent to XML_SetStartElementHandler and XML_SetEndElementHandler calls
    XML_SetCharacterDataHandler(_parser, xml_characterDataHandler);
    XML_SetProcessingInstructionHandler(_parser, xml_processingInstructionHandler);
    XML_SetCommentHandler(_parser, xml_commentHandler);
    XML_SetStartCdataSectionHandler(_parser, xml_startCdataSectionHandler);
    XML_SetEndCdataSectionHandler(_parser, xml_endCdataSectionHandler);
//    XML_SetCdataSectionHandler(_parser, ); // equivalent to XML_SetStartCdataSectionHandler and XML_SetEndCdataSectionHandler calls
    XML_SetDefaultHandler(_parser, xml_defaultHandler);
//    XML_SetDefaultHandlerExpand(_parser);

//    XML_SetExternalEntityRefHandler(_parser);
//    XML_SetExternalEntityRefHandlerArg(_parser);
    XML_SetSkippedEntityHandler(_parser, xml_skippedEntityHandler);
//    XML_SetUnknownEncodingHandler(_parser);

// decl handlers
    XML_SetStartNamespaceDeclHandler(_parser, xml_startNamespaceDeclHandler);
    XML_SetEndNamespaceDeclHandler(_parser, xml_endNamespaceDeclHandler);
//    XML_SetNamespaceDeclHandler(_parser); // equivalent to XML_SetStartNamespaceDeclHandler and XML_SetEndNamespaceDeclHandler calls
    XML_SetXmlDeclHandler(_parser, xml_xmlDeclHandler);
    XML_SetStartDoctypeDeclHandler(_parser, xml_startDoctypeDeclHandler);
    XML_SetEndDoctypeDeclHandler(_parser, xml_endDoctypeDeclHandler);
//    XML_SetDoctypeDeclHandler(_parser); // equivalent to XML_SetStartDoctypeDeclHandler and XML_SetEndDoctypeDeclHandler call
//    XML_SetElementDeclHandler(_parser);
//    XML_SetAttlistDeclHandler(_parser);
//    XML_SetEntityDeclHandler(_parser);
    XML_SetUnparsedEntityDeclHandler(_parser, xml_unparsedEntityDeclHandler);
    XML_SetNotationDeclHandler(_parser, xml_notationDeclHandler);

//    XML_SetNotStandaloneHandler(_parser);
	XML_SetUserData(_parser, this);
}

void reader::impl::split (const XML_Char * str, pfs::string & nsURI, pfs::string & localName)
{
	nsURI.clear();
	localName.clear();

	const XML_Char * p = str;
	const XML_Char * pbegin = str;
	const XML_Char * pend = str + strlen(p);

	while (p != pend) {
		if (*p == NamespaceSeparator) {
			nsURI = pfs::string::fromUtf8(pbegin, p-pbegin);
			pbegin = ++p;
			break;
		}
		++p;
	}

	localName = pfs::string::fromUtf8(pbegin);
}


bool reader::impl::parse (const pfs::string & src)
{
	int isFinal = 1; // this is the last piece of the document
	_handlers->beginDocument();
	XML_Status status = XML_Parse(_parser, src.c_str(), src.sizeInBytes(), isFinal);
	_handlers->endDocument(status == XML_STATUS_OK);
	return status == XML_STATUS_OK;
}

void XMLCALL reader::impl::xml_startElementHandler (
		  void * readerImpl
		, const XML_Char * name
        , const XML_Char ** atts)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);

	if (p->_handlers) {
		pfs::string nsURI;
		pfs::string localName;

		split(name, nsURI, localName);

		pfs::vector<attr_triplet> attributes;

		while (*atts) {
			attr_triplet attr;
			split(*atts++, attr.nsURI, attr.localName);
			attr.value = _u8(*atts++);
			attributes.append(attr);
		}

		p->_handlers->startElement(nsURI, localName, attributes);
	}
}

void XMLCALL reader::impl::xml_endElementHandler (
		  void * readerImpl
		, const XML_Char * name)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		pfs::string nsURI;
		pfs::string localName;
		split(name, nsURI, localName);

		p->_handlers->endElement(nsURI, localName);
	}
}

void XMLCALL reader::impl::xml_characterDataHandler (
		  void * readerImpl
		, const XML_Char * s
		, int len)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		pfs::string text = pfs::string::fromUtf8(s, len);
		p->_handlers->characterData(text);
	}
}

void XMLCALL reader::impl::xml_processingInstructionHandler (
		  void * readerImpl
		, const XML_Char * target
		, const XML_Char * data)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->processingInstruction(
			  pfs::string::fromUtf8(target)
			, pfs::string::fromUtf8(data));
	}
}

void XMLCALL reader::impl::xml_commentHandler (
		  void * readerImpl
		, const XML_Char * data)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->comment(pfs::string::fromUtf8(data));
	}
}

void XMLCALL reader::impl::xml_startCdataSectionHandler (
			void * readerImpl)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->startCdataSection();
	}
}

void XMLCALL reader::impl::xml_endCdataSectionHandler (
			void * readerImpl)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->endCdataSection();
	}
}

void XMLCALL reader::impl::xml_defaultHandler (
			  void * readerImpl
	        , const XML_Char * s
	        , int len)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->defaultData(pfs::string::fromUtf8(s, len));
	}
}

void XMLCALL reader::impl::xml_skippedEntityHandler (
		  void * readerImpl
		, const XML_Char * entityName
		, int is_parameter_entity)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->skippedEntity(pfs::string::fromUtf8(entityName), is_parameter_entity != 0);
	}
}


// The way to distinguish is that the version parameter will be NULL for text declarations.
// The encoding parameter may be NULL for an XML declaration.
// The standalone argument will contain -1, 0, or 1 indicating respectively
// that there was no standalone parameter in the declaration,
// that it was given as no, or
// that it was given as yes.
void XMLCALL reader::impl::xml_xmlDeclHandler (
		  void * readerImpl
		, const XML_Char * version
		, const XML_Char * encoding
		, int standalone)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	int major = -1;
	int minor = -1;

	PFS_UNUSED2(encoding, standalone);

	pfs::string ver(version);
	if(!ver.isEmpty()) {
		pfs::vector<pfs::string> parts = ver.split(pfs::ucchar('.'), true);
		if (parts.size() > 1) {
			bool ok = false;
			major = parts[0].toInt(& ok);
			if (ok) {
				minor = parts[1].toInt(& ok);
			}

			if (!ok)
				major = minor = -1;
		}
	}

	if (!p->_acceptVersion(major, minor)) {
		XML_StopParser(p->_parser, XML_FALSE);
	}
}

void XMLCALL reader::impl::xml_startNamespaceDeclHandler (
		  void * readerImpl
		, const XML_Char * prefix
		, const XML_Char * uri)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->startNamespaceDecl(pfs::string::fromUtf8(prefix), pfs::string::fromUtf8(uri));
	}
}

void XMLCALL reader::impl::xml_endNamespaceDeclHandler (
		  void * readerImpl
		, const XML_Char * prefix)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->endNamespaceDecl(pfs::string::fromUtf8(prefix));
	}
}

void	XMLCALL reader::impl::xml_startDoctypeDeclHandler (
		  void * readerImpl
		, const XML_Char * doctypeName
		, const XML_Char * sysid
		, const XML_Char * pubid
		, int has_internal_subset)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->startDoctypeDecl(_u8(doctypeName), _u8(sysid), _u8(pubid), has_internal_subset != 0);
	}
}

void XMLCALL reader::impl::xml_endDoctypeDeclHandler (void * readerImpl)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->endDoctypeDecl();
	}
}

void XMLCALL reader::impl::xml_unparsedEntityDeclHandler (
			  void * readerImpl
			, const XML_Char * entityName
			, const XML_Char * base
			, const XML_Char * systemId
			, const XML_Char * publicId
			, const XML_Char * notationName)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->unparsedEntityDecl(_u8(entityName)
				, _u8(base)
				, _u8(systemId)
				, _u8(publicId)
				, _u8(notationName));
	}
}

void XMLCALL reader::impl::xml_notationDeclHandler (
		  void * readerImpl
        , const XML_Char * notationName
        , const XML_Char * base
        , const XML_Char * systemId
        , const XML_Char * publicId)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		p->_handlers->notationDecl(_u8(notationName)
				, _u8(base)
				, _u8(systemId)
				, _u8(publicId));
	}
}



}} // cwt::xml
