/**
 * @file reader.hpp
 * @author wladt
 * @date Dec 10, 2013
 */

#ifndef __CWT_XML_SAX_HPP__
#define __CWT_XML_SAX_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/functional.hpp>
#include <cwt/errorable.hpp>
#include <utility>

namespace cwt { namespace xml {

class handlers
{
public:
	handlers () {}
	virtual ~handlers () {}

	virtual void beginDocument () {}
	virtual void endDocument (bool /*success*/) {}
	virtual void startElement (const pfs::string & /*tagname*/, const pfs::map<pfs::string, pfs::string> & /*atts*/) {}
	virtual void endElement (const pfs::string & /*tagname*/) {}
	virtual void characterData (const pfs::string & /*text*/) {}
	virtual void processingInstruction (const pfs::string & /*target*/, const pfs::string & /*data*/) {}
	virtual void comment (const pfs::string & /*text*/) {}
	virtual void startCdataSection () {}
	virtual void endCdataSection () {}
	virtual void defaultData (const pfs::string &) {}
	virtual void skippedEntity (const pfs::string & /*entityName*/, bool /*is_parameter_entity*/) {}
	virtual void startNamespaceDecl (const pfs::string & /*prefix*/, const pfs::string & /*uri*/) {}
	virtual void endNamespaceDecl (const pfs::string & /*prefix*/) {}
	virtual void startDoctypeDecl (const pfs::string & /*doctypeName*/
			, const pfs::string & /*systemId*/
			, const pfs::string & /*publicId*/
			, bool /*has_internal_subset*/) {}
	virtual void endDoctypeDecl () {}
	virtual void unparsedEntityDecl (
			  const pfs::string & /*entityName*/
			, const pfs::string & /*base*/
			, const pfs::string & /*systemId*/
			, const pfs::string & /*publicId*/
			, const pfs::string & /*notationName*/) {}
	virtual void notationDecl (
			  const pfs::string & /*notationName*/
	        , const pfs::string & /*base*/
	        , const pfs::string & /*systemId*/
	        , const pfs::string & /*publicId*/) {}
};

class DLL_API reader : public cwt::errorable
{
	PFS_PIMPL_DECL_NOCC(reader, impl);

public:
	reader (handlers * h = nullptr);

	bool parse (const pfs::string & src);
	void setHandler (handlers * h);
	void acceptVersion (pfs::function<bool (int, int)>);
};

}} // cwt::xml

#endif /* __XML_CWT_SAX_HPP__ */
