/**
 * @file xml_expat.cpp
 * @author wladt
 * @date Dec 9, 2013
 */
#include "xml_expat.hpp"

namespace cwt { namespace xml {

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
	_parser = XML_ParserCreateNS("UTF-8", '\xFF');
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
//    XML_SetSkippedEntityHandler(_parser);
//    XML_SetUnknownEncodingHandler(_parser);

// decl handlers
//    XML_SetStartNamespaceDeclHandler(_parser);
//    XML_SetEndNamespaceDeclHandler(_parser);
//    XML_SetNamespaceDeclHandler(_parser);
    XML_SetXmlDeclHandler(_parser, xml_xmlDeclHandler);
//    XML_SetStartDoctypeDeclHandler(_parser);
//    XML_SetEndDoctypeDeclHandler(_parser);
//    XML_SetDoctypeDeclHandler(_parser);
//    XML_SetElementDeclHandler(_parser);
//    XML_SetAttlistDeclHandler(_parser);
//    XML_SetEntityDeclHandler(_parser);
//    XML_SetUnparsedEntityDeclHandler(_parser);
//    XML_SetNotationDeclHandler(_parser);


//    XML_SetNotStandaloneHandler(_parser);
	XML_SetUserData(_parser, this);
}

bool reader::impl::parse (const pfs::string & src)
{
	int isFinal = 1; // this is the last piece of the document
	XML_Status status = XML_Parse(_parser, src.c_str(), src.sizeInBytes(), isFinal);
	return status == XML_STATUS_OK;
}

void XMLCALL reader::impl::xml_startElementHandler (
		  void * readerImpl
		, const XML_Char * name
        , const XML_Char ** atts)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);

	if (p->_handlers) {
		pfs::string tagname = pfs::string::fromUtf8(name);
		pfs::map<pfs::string, pfs::string> attributes;

		while (*atts) {
			pfs::string attname = pfs::string::fromUtf8(*atts++);
			pfs::string attvalue = pfs::string::fromUtf8(*atts++);
			attributes.insert(attname, attvalue);
		}

		p->_handlers->startElement(tagname, attributes);
	}
}

void XMLCALL reader::impl::xml_endElementHandler (
		  void * readerImpl
		, const XML_Char * name)
{
	reader::impl * p = static_cast<reader::impl *>(readerImpl);
	if (p->_handlers) {
		pfs::string tagname = pfs::string::fromUtf8(name);
		p->_handlers->endElement(tagname);
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


}} // cwt::xml
