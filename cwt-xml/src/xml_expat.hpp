/**
 * @file xml_expat.hpp
 * @author wladt
 * @date Dec 9, 2013
 */

#ifndef __CWT_XML_EXPAT_HPP__
#define __CWT_XML_EXPAT_HPP__

#include "../include/cwt/xml/sax.hpp"
#include "../expat2/lib/expat.h"

#ifndef XMLCALL
#if defined(_MSC_EXTENSIONS) && !defined(__BEOS__) && !defined(__CYGWIN__)
#define XMLCALL __cdecl
#elif defined(__GNUC__)
#define XMLCALL __attribute__((cdecl))
#else
#define XMLCALL
#endif
#endif

namespace cwt { namespace xml {

class reader::impl
{
	friend class reader;

	static const XML_Char NamespaceSeparator;

	XML_Parser _parser;
	handlers * _handlers;
	pfs::function<bool (int, int)> _acceptVersion;

public:
	impl ();
	~impl ();

	void setHandlers (handlers * h) { _handlers = h; }
	bool isError () const;
	pfs::string errorString () const;
	bool parse (const pfs::string & src);

	static void XMLCALL xml_startElementHandler (
			  void * readerImpl
			, const XML_Char * name
	        , const XML_Char ** atts);

	static void XMLCALL xml_endElementHandler (
			  void * readerImpl
			, const XML_Char * name);

	static void	XMLCALL xml_characterDataHandler (
			  void * readerImpl
			, const XML_Char * s
			, int len);

	static void XMLCALL xml_processingInstructionHandler (
			  void * readerImpl
			, const XML_Char * target
			, const XML_Char * data);

	static void XMLCALL xml_commentHandler(
			  void * readerImpl
			, const XML_Char * data);

	static void XMLCALL xml_startCdataSectionHandler (
			void * readerImpl);

	static void XMLCALL xml_endCdataSectionHandler (
			void * readerImpl);

	static void XMLCALL xml_defaultHandler (
			  void * readerImpl
	        , const XML_Char * s
	        , int len);

	static void XMLCALL xml_skippedEntityHandler (
			  void * readerImpl
			, const XML_Char * entityName
			, int is_parameter_entity);

	static void	XMLCALL xml_xmlDeclHandler (
			  void * readerImpl
			, const XML_Char * version
			, const XML_Char * encoding
			, int standalone);

	static void XMLCALL xml_startNamespaceDeclHandler (
			  void * readerImpl
			, const XML_Char * prefix
			, const XML_Char * uri);

	static void XMLCALL xml_endNamespaceDeclHandler (
			  void * readerImpl
			, const XML_Char * prefix);

	static void	XMLCALL xml_startDoctypeDeclHandler (
			  void * readerImpl
			, const XML_Char * doctypeName
			, const XML_Char * sysid
			, const XML_Char * pubid
			, int has_internal_subset);

	static void XMLCALL xml_endDoctypeDeclHandler (void * readerImpl);

	static void XMLCALL xml_unparsedEntityDeclHandler (
			  void * readerImpl
			, const XML_Char * entityName
			, const XML_Char * base
			, const XML_Char * systemId
			, const XML_Char * publicId
			, const XML_Char * notationName);

	static void XMLCALL xml_notationDeclHandler (
			  void * readerImpl
	        , const XML_Char * notationName
	        , const XML_Char * base
	        , const XML_Char * systemId
	        , const XML_Char * publicId);

private:
	static void split (const XML_Char * str, pfs::string & nsURI, pfs::string & localName);
};

inline reader::impl::~impl ()
{
	PFS_ASSERT(_parser);
	XML_ParserFree(_parser);
	_parser = nullptr;
	_handlers = nullptr;
}

inline pfs::string reader::impl::errorString () const
{
	pfs::string r;
	XML_Error ecode = XML_GetErrorCode(_parser);
	XML_Size lineno = XML_GetCurrentLineNumber(_parser);
	XML_Size colno  = XML_GetCurrentColumnNumber(_parser);
	r << pfs::string::fromUtf8(XML_ErrorString(ecode));
	r << " at line " << pfs::string::number(lineno) << " column " << pfs::string::number(colno);
	return r;
}

inline bool reader::impl::isError () const
{
	return XML_GetErrorCode(_parser) != XML_ERROR_NONE;
}

}} // cwt::xml

#endif /* __CWT_XML_EXPAT_HPP__ */
