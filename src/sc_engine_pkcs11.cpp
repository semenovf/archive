/*
 * SmartCardContext.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#include "pkcs11.h"
#include <jq/dl.hpp>
#include <jq/smartcard.hpp>

static const char_type* __E_SC_PKCS11_LOADMODULE = _Tr("Unable to load PKCS11 module: %s");
static const char_type* __E_SC_PKCS11_GETFUNCLIST = _Tr("Failed to retrieve 'C_GetFunctionList'");
static const char_type* __E_SC_PKCS11_APIFUNCLOAD = _Tr("Failed to obtain the table of PKCS11 API functions");

extern bool __IS_INVALID_CONTEXT(jq::SmartCardContext*);
#define __CHECK_CONTEXT(pContext) if(__IS_INVALID_CONTEXT(pContext)){return;}
#define __CHECK_CONTEXT_RV(pContext,rv) if(__IS_INVALID_CONTEXT(pContext)){return (rv);}

JQ_NS_BEGIN

P11* SmartCard::p11Create(char_type *path) const
{
	__CHECK_CONTEXT_RV(m_pContext, false);

	Dl* module;
	CK_RV rv;
	CK_RV (*c_get_function_list)(CK_FUNCTION_LIST_PTR_PTR);
	CK_FUNCTION_LIST_PTR api = NULL;

	module = new Dl();
	JQ_ASSERT(module);

	do {
		if( !module->open(path) ) {
			String emsg;
			emsg.sprintf(__E_SC_PKCS11_LOADMODULE)(path);
			m_pContext->setError(emsg);
			break;
		}

		c_get_function_list =
			(CK_RV (*)(CK_FUNCTION_LIST_PTR_PTR))
			module->symbol("C_GetFunctionList");

		if( !c_get_function_list ) {
			m_pContext->setError(__E_SC_PKCS11_GETFUNCLIST);
			break;
		}

		rv = c_get_function_list( &api );
		if ( rv != CKR_OK ) {
			m_pContext->setError(__E_SC_PKCS11_APIFUNCLOAD);
		}

		return new P11(module, api);
	} while(0);

	if( module ) {
		module->close();
		delete module;
	}
	return NULL;
}

void SmartCard::p11Destroy(P11* session) const
{
	if( session )
		delete session;
}

P11::~P11()
{
	JQ_DEBUG("P11::~P11()");
	if( m_module ) {
		m_module->close();
		delete m_module;
		m_module = NULL;
	}
}

void P11::version(uchar_t &major, uchar_t &minor)
{
	JQ_ASSERT(m_api);
	CK_FUNCTION_LIST_PTR api = (CK_FUNCTION_LIST_PTR)m_api;
	major = api->version.major;
	minor = api->version.minor;
}

JQ_NS_END
