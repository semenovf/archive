/**
 * @file   abnf_p.hpp
 * @author wladt
 * @date   Feb 18, 2013 9:10:27 AM
 *
 * @brief
 */

#ifndef __CWT_ABNF_P_HPP__
#define __CWT_ABNF_P_HPP__

#include "cwt/abnf.hpp"
#include <cwt/callback.hpp>

CWT_NS_BEGIN

#define _CAST_CTX(ctx) reinterpret_cast<AbnfParseContext*>(ctx)

struct AbnfParseContext {
	Errorable     *status;
	void          *userContext;

	Callback2<const String&, void*> on_rulename;
	Callback2<const String&, void*> on_comment;
	Callback1<void*> on_begin_option;
	Callback1<void*> on_end_option;
	Callback1<void*> on_begin_group;
	Callback1<void*> on_end_group;
	Callback2<const String&, void*> on_numeric_value;
	Callback2<const String&, void*> on_prose_value;
};

CWT_NS_END

#endif /* __CWT_ABNF_P_HPP__ */
