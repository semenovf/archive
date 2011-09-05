/*
 * SmartCardContext.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#include <jq/errorable.hpp>
#include <jq/pkcs11.hpp>

static const char_type* __E_SC_PKCS11_LOADMODULE   = _Tr("unable to load PKCS11 module: %s");
static const char_type* __E_SC_PKCS11_GETFUNCLIST  = _Tr("failed to retrieve 'C_GetFunctionList'");
static const char_type* __E_SC_PKCS11_APILOAD      = _Tr("failed to obtain the table of PKCS11 API functions");
static const char_type* __E_SC_PKCS11_INITIALIZE   = _Tr("failed to initialize PKCS11");
static const char_type* __E_SC_PKCS11_GETINFO      = _Tr("failed to get PKCS11 info");
static const char_type* __E_SC_PKCS11_OOBSLOTINDEX = _Tr("slot index is out of bounds");
static const char_type* __E_SC_PKSC11_SLOTLIST     = _Tr("failed to get slot list");
static const char_type* __E_SC_PKCS11_SLOTINFO     = _Tr("failed to get slot info");
static const char_type* __E_SC_PKCS11_OPENSESSION  = _Tr("failed to open session");
static const char_type* __E_SC_PKCS11_CLOSESESSION = _Tr("unable to close session");

//extern bool __IS_INVALID_CONTEXT(jq::SmartCardContext*);
/*
#define __CHECK_CONTEXT(pContext) if(__IS_INVALID_CONTEXT(pContext)){return;}
#define __CHECK_CONTEXT_RV(pContext,rv) if(__IS_INVALID_CONTEXT(pContext)){return (rv);}
*/

JQ_NS_BEGIN

static const char* __ckrvToString(CK_RV res);

static void __set_pkcs11_error(CK_RV rv, const String& msg)
{
	String emsg;
	emsg.sprintf("%s: %s (0x%0lX)")(msg)(__ckrvToString(rv))(rv);
	jq_emitError(emsg);
}


Pkcs11* Pkcs11::open(char_type *path)
{
//	__CHECK_CONTEXT_RV(m_pContext, false);

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
			jq_emitError(emsg.c_str());
			break;
		}

		c_get_function_list =
			(CK_RV (*)(CK_FUNCTION_LIST_PTR_PTR))
			module->symbol("C_GetFunctionList");

		if( !c_get_function_list ) {
			jq_emitError(__E_SC_PKCS11_GETFUNCLIST);
			break;
		}

		rv = c_get_function_list( &api );
		if ( rv != CKR_OK ) {
			jq_emitError(__E_SC_PKCS11_APILOAD);
			break;
		}

		rv = ((CK_FUNCTION_LIST_PTR)api)->C_Initialize(NULL);
		if(rv != CKR_OK) {
			jq_emitError(__E_SC_PKCS11_INITIALIZE);
			break;
		}

		return new Pkcs11(module, api);
	} while(0);

	if( module ) {
		module->close();
		delete module;
	}
	return NULL;
}

void Pkcs11::close(Pkcs11* p11)
{
	if( p11 )
		delete p11;
}

Pkcs11::~Pkcs11()
{
	JQ_DEBUG("Pkcs11::~Pkcs11()");
	JQ_ASSERT(m_module);

	m_api->C_Finalize(NULL_PTR);
	m_module->close();
	delete m_module;
	m_module = NULL;
}


/**
 *
 * @param cryptokiMajor   cryptoki version major number
 * @param cryptokiMinor   cryptoki version minor number
 * @param manufacturerId  manufacturer identifier
 * @param libDesc         library description
 * @param libMajor        library version major number
 * @param libMinor        library version minor number
 * @return @c true if ok, @c false otherwise
 */
bool Pkcs11::info( uchar_t* cryptokiMajor
	, uchar_t* cryptokiMinor
	, String* manufacturerId
	, String* libDesc
	, uchar_t* libMajor
	, uchar_t* libMinor)
{
	JQ_ASSERT(m_api);
	CK_INFO info;
	CK_RV rv = m_api->C_GetInfo(&info);
	if(rv != CKR_OK) {
		__set_pkcs11_error(rv, __E_SC_PKCS11_GETINFO);
		return false;
	}
	if( cryptokiMajor )
		*cryptokiMajor = info.cryptokiVersion.major;
	if( cryptokiMinor )
		*cryptokiMinor = info.cryptokiVersion.minor;
	if( manufacturerId )
		*manufacturerId = (char_type*)info.manufacturerID;
	if( libDesc )
		*libDesc = (char_type*)info.libraryDescription;
	if( libMajor )
		*libMajor = info.libraryVersion.major;
	if( libMinor )
		*libMinor = info.libraryVersion.minor;
	return true;
}


void Pkcs11::version(uchar_t &major, uchar_t &minor)
{
	JQ_ASSERT(m_api);
	major = m_api->version.major;
	minor = m_api->version.minor;
}


/**
 *
 * @param tokenPresent @c true - updates only slots with token present
 */
void Pkcs11::updateSlots(bool tokenPresent)
{
	CK_ULONG nslots = 0;
	CK_SLOT_ID *pslots = NULL;
//	CK_SLOT_INFO info;

	CK_RV rv = m_api->C_GetSlotList(tokenPresent, NULL, &nslots);

	if( rv != CKR_OK ) {
		__set_pkcs11_error(rv, __E_SC_PKSC11_SLOTLIST);
		return;
	}

	pslots = JQ_MALLOC(CK_SLOT_ID, nslots * sizeof(CK_SLOT_ID));
	JQ_ASSERT(pslots);

	rv = m_api->C_GetSlotList(tokenPresent, pslots, &nslots);
	if( rv != CKR_OK ) {
		__set_pkcs11_error(rv, __E_SC_PKSC11_SLOTLIST);
	} else {
		for( uint_t i = 0; i < nslots; i++ ) {
			m_slots.push_back(pslots[i]);
		}
	}

	if( pslots ) {
		JQ_FREE(pslots);
	}

}


bool Pkcs11::slotInfo(uint_t index, CK_SLOT_INFO& info)
{
	if( index >= m_slots.size() ) {
		jq_emitError(__E_SC_PKCS11_OOBSLOTINDEX);
		return false;
	}

	CK_RV rv = m_api->C_GetSlotInfo(m_slots[index], &info);
	if( rv != CKR_OK ) {
		__set_pkcs11_error(rv, __E_SC_PKCS11_SLOTINFO);
		return false;
	}

	return true;
}


Pkcs11Session* Pkcs11::openSession(uint_t slotIndex) const
{
	if( slotIndex >= m_slots.size() ) {
		jq_emitError(__E_SC_PKCS11_OOBSLOTINDEX);
		return false;
	}

	CK_SESSION_HANDLE session;
	CK_FLAGS flags = CKF_SERIAL_SESSION;
	CK_RV rv = m_api->C_OpenSession(
		m_slots[slotIndex]
	    , flags
	    , NULL/*void *application*/
		, NULL/*ck_notify_t notify*/
	    , &session);
	if( rv != CKR_OK ) {
		__set_pkcs11_error(rv, __E_SC_PKCS11_OPENSESSION);
		return NULL;
	}
	return new Pkcs11Session(session);
}


void Pkcs11::closeSession(Pkcs11Session* session) const
{
	if( session ) {
		CK_RV rv = m_api->C_CloseSession( session->m_session );
		if( rv != CKR_OK ) {
			__set_pkcs11_error(rv, __E_SC_PKCS11_CLOSESESSION);
		}
		delete session;
	}
}


const char* __ckrvToString(CK_RV res)
{
	switch (res) {
	case CKR_OK:
		return "CKR_OK";
	case CKR_CANCEL:
		return "CKR_CANCEL";
	case CKR_HOST_MEMORY:
		return "CKR_HOST_MEMORY";
	case CKR_SLOT_ID_INVALID:
		return "CKR_SLOT_ID_INVALID";
	case CKR_GENERAL_ERROR:
		return "CKR_GENERAL_ERROR";
	case CKR_FUNCTION_FAILED:
		return "CKR_FUNCTION_FAILED";
	case CKR_ARGUMENTS_BAD:
		return "CKR_ARGUMENTS_BAD";
	case CKR_NO_EVENT:
		return "CKR_NO_EVENT";
	case CKR_NEED_TO_CREATE_THREADS:
		return "CKR_NEED_TO_CREATE_THREADS";
	case CKR_CANT_LOCK:
		return "CKR_CANT_LOCK";
	case CKR_ATTRIBUTE_READ_ONLY:
		return "CKR_ATTRIBUTE_READ_ONLY";
	case CKR_ATTRIBUTE_SENSITIVE:
		return "CKR_ATTRIBUTE_SENSITIVE";
	case CKR_ATTRIBUTE_TYPE_INVALID:
		return "CKR_ATTRIBUTE_TYPE_INVALID";
	case CKR_ATTRIBUTE_VALUE_INVALID:
		return "CKR_ATTRIBUTE_VALUE_INVALID";
	case CKR_DATA_INVALID:
		return "CKR_DATA_INVALID";
	case CKR_DATA_LEN_RANGE:
		return "CKR_DATA_LEN_RANGE";
	case CKR_DEVICE_ERROR:
		return "CKR_DEVICE_ERROR";
	case CKR_DEVICE_MEMORY:
		return "CKR_DEVICE_MEMORY";
	case CKR_DEVICE_REMOVED:
		return "CKR_DEVICE_REMOVED";
	case CKR_ENCRYPTED_DATA_INVALID:
		return "CKR_ENCRYPTED_DATA_INVALID";
	case CKR_ENCRYPTED_DATA_LEN_RANGE:
		return "CKR_ENCRYPTED_DATA_LEN_RANGE";
	case CKR_FUNCTION_CANCELED:
		return "CKR_FUNCTION_CANCELED";
	case CKR_FUNCTION_NOT_PARALLEL:
		return "CKR_FUNCTION_NOT_PARALLEL";
	case CKR_FUNCTION_NOT_SUPPORTED:
		return "CKR_FUNCTION_NOT_SUPPORTED";
	case CKR_KEY_HANDLE_INVALID:
		return "CKR_KEY_HANDLE_INVALID";
	case CKR_KEY_SIZE_RANGE:
		return "CKR_KEY_SIZE_RANGE";
	case CKR_KEY_TYPE_INCONSISTENT:
		return "CKR_KEY_TYPE_INCONSISTENT";
	case CKR_KEY_NOT_NEEDED:
		return "CKR_KEY_NOT_NEEDED";
	case CKR_KEY_CHANGED:
		return "CKR_KEY_CHANGED";
	case CKR_KEY_NEEDED:
		return "CKR_KEY_NEEDED";
	case CKR_KEY_INDIGESTIBLE:
		return "CKR_KEY_INDIGESTIBLE";
	case CKR_KEY_FUNCTION_NOT_PERMITTED:
		return "CKR_KEY_FUNCTION_NOT_PERMITTED";
	case CKR_KEY_NOT_WRAPPABLE:
		return "CKR_KEY_NOT_WRAPPABLE";
	case CKR_KEY_UNEXTRACTABLE:
		return "CKR_KEY_UNEXTRACTABLE";
	case CKR_MECHANISM_INVALID:
		return "CKR_MECHANISM_INVALID";
	case CKR_MECHANISM_PARAM_INVALID:
		return "CKR_MECHANISM_PARAM_INVALID";
	case CKR_OBJECT_HANDLE_INVALID:
		return "CKR_OBJECT_HANDLE_INVALID";
	case CKR_OPERATION_ACTIVE:
		return "CKR_OPERATION_ACTIVE";
	case CKR_OPERATION_NOT_INITIALIZED:
		return "CKR_OPERATION_NOT_INITIALIZED";
	case CKR_PIN_INCORRECT:
		return "CKR_PIN_INCORRECT";
	case CKR_PIN_INVALID:
		return "CKR_PIN_INVALID";
	case CKR_PIN_LEN_RANGE:
		return "CKR_PIN_LEN_RANGE";
	case CKR_PIN_EXPIRED:
		return "CKR_PIN_EXPIRED";
	case CKR_PIN_LOCKED:
		return "CKR_PIN_LOCKED";
	case CKR_SESSION_CLOSED:
		return "CKR_SESSION_CLOSED";
	case CKR_SESSION_COUNT:
		return "CKR_SESSION_COUNT";
	case CKR_SESSION_HANDLE_INVALID:
		return "CKR_SESSION_HANDLE_INVALID";
	case CKR_SESSION_PARALLEL_NOT_SUPPORTED:
		return "CKR_SESSION_PARALLEL_NOT_SUPPORTED";
	case CKR_SESSION_READ_ONLY:
		return "CKR_SESSION_READ_ONLY";
	case CKR_SESSION_EXISTS:
		return "CKR_SESSION_EXISTS";
	case CKR_SESSION_READ_ONLY_EXISTS:
		return "CKR_SESSION_READ_ONLY_EXISTS";
	case CKR_SESSION_READ_WRITE_SO_EXISTS:
		return "CKR_SESSION_READ_WRITE_SO_EXISTS";
	case CKR_SIGNATURE_INVALID:
		return "CKR_SIGNATURE_INVALID";
	case CKR_SIGNATURE_LEN_RANGE:
		return "CKR_SIGNATURE_LEN_RANGE";
	case CKR_TEMPLATE_INCOMPLETE:
		return "CKR_TEMPLATE_INCOMPLETE";
	case CKR_TEMPLATE_INCONSISTENT:
		return "CKR_TEMPLATE_INCONSISTENT";
	case CKR_TOKEN_NOT_PRESENT:
		return "CKR_TOKEN_NOT_PRESENT";
	case CKR_TOKEN_NOT_RECOGNIZED:
		return "CKR_TOKEN_NOT_RECOGNIZED";
	case CKR_TOKEN_WRITE_PROTECTED:
		return "CKR_TOKEN_WRITE_PROTECTED";
	case CKR_UNWRAPPING_KEY_HANDLE_INVALID:
		return "CKR_UNWRAPPING_KEY_HANDLE_INVALID";
	case CKR_UNWRAPPING_KEY_SIZE_RANGE:
		return "CKR_UNWRAPPING_KEY_SIZE_RANGE";
	case CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT:
		return "CKR_UNWRAPPING_KEY_TYPE_INCONSISTENT";
	case CKR_USER_ALREADY_LOGGED_IN:
		return "CKR_USER_ALREADY_LOGGED_IN";
	case CKR_USER_NOT_LOGGED_IN:
		return "CKR_USER_NOT_LOGGED_IN";
	case CKR_USER_PIN_NOT_INITIALIZED:
		return "CKR_USER_PIN_NOT_INITIALIZED";
	case CKR_USER_TYPE_INVALID:
		return "CKR_USER_TYPE_INVALID";
	case CKR_USER_ANOTHER_ALREADY_LOGGED_IN:
		return "CKR_USER_ANOTHER_ALREADY_LOGGED_IN";
	case CKR_USER_TOO_MANY_TYPES:
		return "CKR_USER_TOO_MANY_TYPES";
	case CKR_WRAPPED_KEY_INVALID:
		return "CKR_WRAPPED_KEY_INVALID";
	case CKR_WRAPPED_KEY_LEN_RANGE:
		return "CKR_WRAPPED_KEY_LEN_RANGE";
	case CKR_WRAPPING_KEY_HANDLE_INVALID:
		return "CKR_WRAPPING_KEY_HANDLE_INVALID";
	case CKR_WRAPPING_KEY_SIZE_RANGE:
		return "CKR_WRAPPING_KEY_SIZE_RANGE";
	case CKR_WRAPPING_KEY_TYPE_INCONSISTENT:
		return "CKR_WRAPPING_KEY_TYPE_INCONSISTENT";
	case CKR_RANDOM_SEED_NOT_SUPPORTED:
		return "CKR_RANDOM_SEED_NOT_SUPPORTED";
	case CKR_RANDOM_NO_RNG:
		return "CKR_RANDOM_NO_RNG";
	case CKR_DOMAIN_PARAMS_INVALID:
		return "CKR_DOMAIN_PARAMS_INVALID";
	case CKR_BUFFER_TOO_SMALL:
		return "CKR_BUFFER_TOO_SMALL";
	case CKR_SAVED_STATE_INVALID:
		return "CKR_SAVED_STATE_INVALID";
	case CKR_INFORMATION_SENSITIVE:
		return "CKR_INFORMATION_SENSITIVE";
	case CKR_STATE_UNSAVEABLE:
		return "CKR_STATE_UNSAVEABLE";
	case CKR_CRYPTOKI_NOT_INITIALIZED:
		return "CKR_CRYPTOKI_NOT_INITIALIZED";
	case CKR_CRYPTOKI_ALREADY_INITIALIZED:
		return "CKR_CRYPTOKI_ALREADY_INITIALIZED";
	case CKR_MUTEX_BAD:
		return "CKR_MUTEX_BAD";
	case CKR_MUTEX_NOT_LOCKED:
		return "CKR_MUTEX_NOT_LOCKED";
	case CKR_VENDOR_DEFINED:
		return "CKR_VENDOR_DEFINED";
	}
	return _Tr("unknown PKCS11 error");
}

JQ_NS_END
