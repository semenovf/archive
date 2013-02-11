/*
 * abnf.hpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */

#ifndef __CWT_ABNF_HPP__
#define __CWT_ABNF_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/errorable.hpp>
/*
#include <cwt/hash.hpp>
#include <cwt/vector.hpp>
*/

CWT_NS_BEGIN

class DLL_API Abnf : public Errorable {
public:
	Abnf() : Errorable() {}
	~Abnf() {}

	bool parse(const String &abnf);
};

CWT_NS_END

#endif /* __CWT_ABNF_HPP__ */
