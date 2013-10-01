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
#include <cwt/callback.hpp>

CWT_NS_BEGIN

struct AbnfSimpleApi {
	Callback1<void *> beginDocument;
	Callback2<bool, void *> endDocument;
	Callback3<const String &, bool, void *> beginRule; // callback(rulename, incremental, abnfContext)
	Callback1<void *> endRule;
	Callback1<void *> beginAlternation;
	Callback1<void *> endAlternation;
	Callback1<void *> beginConcatenation;
	Callback1<void *> endConcatenation;
	Callback3<int, int, void *> beginRepetition; // callback(from, to, abnfContext)
	Callback1<void *> endRepetition;
	Callback1<void *> beginOption;
	Callback1<void *> endOption;
	Callback1<void *> beginGroup;
	Callback1<void *> endGroup;
	Callback2<const String &, void *> ruleRef;
	Callback2<const String &, void *> charVal;
	Callback2<const String &, void *> numVal;
	Callback2<const String &, void *> proseVal;

	Callback2<const String &, void *> comment;
};

class DLL_API Abnf
{
public:
	Abnf() {}
	~Abnf() {}

	bool parse(const String & abnf);
	bool parse(const String & abnf, AbnfSimpleApi & api);
};

CWT_NS_END

#endif /* __CWT_ABNF_HPP__ */
