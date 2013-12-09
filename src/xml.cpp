/**
 * @file xml.cpp
 * @author wladt
 * @date Dec 9, 2013
 */

#include "xml_p.hpp"

namespace cwt { namespace xml {

reader::reader (handlers * h) : _pimpl(new reader::impl)
{
	_pimpl->setHandlers(h);
}


void reader::setHandler (handlers * h)
{
	_pimpl->setHandlers(h);
}

void reader::setAcceptVersion (pfs::function<bool (int, int)> acceptor)
{
	_pimpl->_acceptVersion = acceptor;
}

bool reader::parse (const pfs::string & src)
{
	return _pimpl->parse(src);
}

bool reader::isError () const
{
	return _pimpl->isError();
}

pfs::string reader::errorString () const
{
	return _pimpl->errorString();
}

}} // cwt::xml
