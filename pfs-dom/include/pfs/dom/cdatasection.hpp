/**
 * @file cdatasection.hpp
 * @author wladt
 * @date Dec 2, 2013
 */

#ifndef __PFS_DOM_CDATASECTION_HPP__
#define __PFS_DOM_CDATASECTION_HPP__

#include <pfs/dom/text.hpp>

namespace pfs { namespace dom {

class text;
class document;
class cdatasection_impl;

class DLL_API cdatasection : public text
{
	friend class document;

protected:
	cdatasection (cdatasection_impl *);

public:
	cdatasection ();
	cdatasection (const cdatasection & other);
	cdatasection & operator = (const cdatasection & other);
};

}} // pfs::dom

#endif /* __PFS_DOM_CDATASECTION_HPP__ */
