#ifndef __CWT_XML_HPP__
#define __CWT_XML_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/functional.hpp>
#include <utility>

namespace cwt { namespace xml {

class DLL_API handlers
{
public:
	handlers () {}
	virtual ~handlers () {}

	virtual void startElement (const pfs::string &, const pfs::map<pfs::string, pfs::string> &) {}
	virtual void endElement (const pfs::string &) {}
	virtual void characterData (const pfs::string &) {}
	virtual void processingInstruction (const pfs::string & , const pfs::string & ) {}
	virtual void comment (const pfs::string &) {}
	virtual void startCdataSection () {}
	virtual void endCdataSection () {}
	virtual void defaultData (const pfs::string &) {}
};

class DLL_API reader
{
	PFS_PIMPL_DECL(reader, impl);

public:
	reader (handlers * h = nullptr);

	bool parse (const pfs::string & src);
	bool isError () const;
	pfs::string errorString () const;
	void setHandler (handlers * h);
	void acceptVersion (pfs::function<bool (int, int)>);
};

}} // cwt::xml

#endif /* __CWT_XML_HPP__ */
