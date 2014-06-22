/*
 * abnf.hpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */

#ifndef __CWT_ABNF_HPP__
#define __CWT_ABNF_HPP__

#include <pfs/string.hpp>
#include <pfs/pimpl.hpp>
#include <cwt/errorable.hpp>

namespace cwt { namespace abnf {

class handlers
{
public:
	handlers () {}
	virtual ~handlers () {}

	virtual void beginDocument () {}
	virtual void endDocument (bool /*success*/) {}
	virtual void beginRule (const pfs::string & rulename, bool incremental) {}
	virtual void endRule () {}
	virtual void beginAlternation () {}
	virtual void endAlternation () {}
	virtual void beginConcatenation () {}
	virtual void endConcatenation () {}
	virtual void beginRepetition (int /*from*/, int /*to*/) {}
	virtual void endRepetition () {}
	virtual void beginOption () {}
	virtual void endOption () {}
	virtual void beginGroup () {}
	virtual void endGroup () {}
	virtual void ruleRef (const pfs::string &) {}
	virtual void charVal (const pfs::string &) {}
	virtual void numVal (const pfs::string &) {}
	virtual void proseVal (const pfs::string &) {}

	virtual void comment (const pfs::string &) {}
};

class DLL_API reader : public cwt::errorable
{
	class impl;
	pfs::pimpl _d;

public:
	reader (handlers * h = nullptr);

	bool parse (const pfs::string & src);
	void setHandlers (handlers * h);
};

}} // cwt::abnf

#endif /* __CWT_ABNF_HPP__ */
