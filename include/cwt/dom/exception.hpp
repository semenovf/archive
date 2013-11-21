/*
 * error.hpp
 *
 *  Created on: Nov 20, 2013
 *      Author: wladt
 */

#ifndef __CWT_DOM_ERROR_HPP__
#define __CWT_DOM_ERROR_HPP__

#include <cwt/cwt.hpp>

namespace cwt { namespace dom {

class exception
{
public:
	static const ushort_t INDEX_SIZE_ERR              = 1;
	static const ushort_t DOMSTRING_SIZE_ERR          = 2;
	static const ushort_t HIERARCHY_REQUEST_ERR       = 3;
	static const ushort_t WRONG_DOCUMENT_ERR          = 4;
	static const ushort_t INVALID_CHARACTER_ERR       = 5;
	static const ushort_t NO_DATA_ALLOWED_ERR         = 6;
	static const ushort_t NO_MODIFICATION_ALLOWED_ERR = 7;
	static const ushort_t NOT_FOUND_ERR               = 8;
	static const ushort_t NOT_SUPPORTED_ERR           = 9;
	static const ushort_t INUSE_ATTRIBUTE_ERR         = 10;
	static const ushort_t INVALID_STATE_ERR           = 11;
	static const ushort_t SYNTAX_ERR                  = 12;
	static const ushort_t INVALID_MODIFICATION_ERR    = 13;
	static const ushort_t NAMESPACE_ERR               = 14;
	static const ushort_t INVALID_ACCESS_ERR          = 15;

private:
	exception();

public:
	exception (ushort_t code) : _code(code) {}

private:
	ushort_t _code;
};

}} // cwt::dom

#endif /* __CWT_DOM_ERROR_HPP__ */
