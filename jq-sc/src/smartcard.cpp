/*
 * SmartCardContext.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#include <sys/stat.h>
#include <jq/logger.hpp>
#include <jq/smartcard.hpp>
#ifdef JQ_WIN32
#	include <winsmcrd.h>
#elif define PCSCD
#	include <PCSC/reader.h>
#else
#	error "Smart card support for this platform unavailable"
#endif

static const char* NtfyReader_ = "\\\\?PnP?\\Notification";

static const char_type* E_SC_Init         = _Tr("cannot connect to resource manager");
static const char_type* E_SC_BadContext   = _Tr("context is invalid");
static const char_type* E_SC_Conn         = _Tr("connection failed");
static const char_type* E_SC_FreeMem      = _Tr("memory deallocation failed");
static const char_type* E_SC_Status       = _Tr("unable to fetch card status");
static const char_type* E_SC_ChStatus     = _Tr("unable to get status change");
static const char_type* E_SC_Cancel       = _Tr("fail to cancel all pending blocking requests");
static const char_type* E_SC_ReadFeatures = _Tr("reading of features failed");
static const char_type* E_SC_Reconnect    = _Tr("reconnection failed");

#define CHECK_CONTEXT(pContext) if(IS_INVALID_CONTEXT(pContext)){return;}
#define CHECK_CONTEXT_RV(pContext,rv) if(IS_INVALID_CONTEXT(pContext)){return (rv);}

bool IS_INVALID_CONTEXT(jq::SmartCardContext* pContext)
{
	JQ_ASSERT(pContext);
/*
	if( pContext->isError() )
		return true;
*/
	if( !pContext->isValid() ) {
		jq_emitError(E_SC_BadContext);
        return true;
	}
	return false;
}

JQ_NS_BEGIN

static jq::String _stringify_error(LONG rv)
{
#ifdef JQ_WIN32
	rv;
	return jq::String();
#else
	return jq::String(pcsc_stringify_error(rv));
#endif
}

static inline void _jq_emitSmartCardError(LONG rv, const String& msg)
{
	jq_emitError(String().sprintf(_Tr("%s: %s (0x%0lX)"))(msg)(_stringify_error(rv))(rv)());
}

static SmartCard::Protocol _fromNativeProto(DWORD proto)
{
	return proto == SCARD_PROTOCOL_T0
		?  SmartCard::Proto_T0
		: ( proto == SCARD_PROTOCOL_T1
			? SmartCard::Proto_T1
			:(proto == SCARD_PROTOCOL_RAW ? SmartCard::Proto_Raw : SmartCard::Proto_Undefined));

}

/**
 *
 */
SmartCardContext::SmartCardContext() : m_context(0L)
{
	JQ_DEBUG("SmartCardContext::SmartCardContext()");

	LONG rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &m_context);
	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(rv, E_SC_Init);
	} else {
		updateReaders();
	}
}

SmartCardContext::~SmartCardContext()
{
	JQ_DEBUG("SmartCardContext::~SmartCardContext()");
	LONG rv = SCardReleaseContext(m_context);
	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(rv, _Tr("SCardReleaseContext: releasing smart card context failed"));
	}
}


/**
 * Checks if smart card context is valid
 *
 * @return
 */
bool SmartCardContext::isValid()
{
	if( SCardIsValidContext(m_context) != SCARD_S_SUCCESS ) {
		return false;
	}
	return true;
}

void SmartCardContext::updateReaders()
{
	CHECK_CONTEXT(this);

	char* readersBuf = NULL;
	uint32_t readersBufSz = SCARD_AUTOALLOCATE;  // Size of multi-string buffer of readers including NULL's.
	LONG rv = SCARD_S_SUCCESS;

	m_readers.clear();

	/* Retrieve the available readers list. */
	while( true ) {
		rv = SCardListReaders(m_context, NULL, /*(char*)*/(LPTSTR)&readersBuf, (DWORD*)&readersBufSz);
		if (rv != SCARD_S_SUCCESS) {
			_jq_emitSmartCardError(rv, "SCardListReaders: fetching available readers list failed");
			break;
		}

		/* fill the readers table */
		char *ptr = readersBuf;
		while( *ptr != _T('\0') ) {
			m_readers.push_back(ptr);
			ptr += strlen(ptr)+1;
		}

		break;
	}

	if( readersBuf /*m_readers.size()*/ ) {
		 rv = SCardFreeMemory(m_context, readersBuf);
		 if (rv != SCARD_S_SUCCESS) {
			 _jq_emitSmartCardError(rv, E_SC_FreeMem);
		 }
	}
}


/**
 * Wait for a reader event (reader added or removed) with special
 * reader name "\\?PnP?\Notification". If a reader event occurs the state of this
 * reader will change and the bit SCARD_STATE_CHANGED will be set.
 *
 * @param millis Maximum waiting time (in milliseconds) for status change, zero (or INFINITE) for infinite.
 * @return true on successful, false on error
 */
bool SmartCardContext::waitAnyReader(ulong_t millis, bool *timedout)
{
	CHECK_CONTEXT_RV(this, false);
	SCARD_READERSTATE readerStates[1];

	if( timedout ) {
		*timedout = false;
	}
	readerStates[0].szReader = (LPCTSTR)NtfyReader_;
	readerStates[0].dwCurrentState = SCARD_STATE_UNAWARE;

	LONG rv = SCardGetStatusChange(m_context, millis, readerStates, 1);
	 if (rv != SCARD_S_SUCCESS) {
		 if( rv == SCARD_E_TIMEOUT && timedout ) {
			 *timedout = true;
		 } else {
			 _jq_emitSmartCardError(rv, E_SC_ChStatus );
		 }
		 return false;
	 }

	JQ_DEBUGF(_Tr("SmartCardContext::waitAnyReader(%ld):"), millis);
	JQ_DEBUGF(_Tr("\tReader: %s"), (const char*)readerStates[0].szReader);
	JQ_DEBUGF(_Tr("\tCurrent state: 0x%04lX"), readerStates[0].dwCurrentState);
	JQ_DEBUGF(_Tr("\tEvent state: 0x%04lX"), readerStates[0].dwEventState);
	return true;
}


/**
 * Cancels all pending blocking requests on the waitAnyReader method ...
 *
 * @return
 */
bool SmartCardContext::cancel()
{
	CHECK_CONTEXT_RV(this, false);
	LONG rv = SCardCancel(m_context);
	 if (rv != SCARD_S_SUCCESS) {
		 _jq_emitSmartCardError(rv, E_SC_Cancel );
		 return false;
	 }
	 return true;
}

/**
 *
 * @param smode
 * @param preferred
 * @return
 */
bool SmartCard::connect(ShareMode smode, Protocol preferred)
{
	CHECK_CONTEXT_RV(m_pContext, false);

	DWORD sm = (smode == SM_Exclusive)
		?  SCARD_SHARE_EXCLUSIVE
		: ((smode == SM_Shared)
		  	  ? SCARD_SHARE_SHARED
		  	  : SCARD_SHARE_DIRECT );
	DWORD m_preferredProto = 0;
	DWORD proto;
	m_preferredProto |= (preferred & Proto_T0 ? SCARD_PROTOCOL_T0 : 0);
	m_preferredProto |= (preferred & Proto_T1 ? SCARD_PROTOCOL_T1 : 0);
	m_preferredProto |= (preferred & Proto_Raw ? SCARD_PROTOCOL_RAW : 0);

	if( m_preferredProto == 0 && sm != SCARD_SHARE_DIRECT ) {
		jq_emitError(_Tr("invalid value for preferred protocol"));
		return false;
	}

	LONG rv = SCardConnect(m_pContext->m_context, m_readerName.c_str(), sm, m_preferredProto, &m_handle, &proto);
	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(rv, E_SC_Conn);
		return false;
	}
	return true;
}


static DWORD _act_mapping[] = { SCARD_LEAVE_CARD, SCARD_RESET_CARD, SCARD_UNPOWER_CARD, SCARD_EJECT_CARD };
static const char_type* _act_str[] = { _Tr("disconnect"), _Tr("reset"), _Tr("unpower"), _Tr("eject") };

/**
 * Do disconnection action
 */
bool SmartCard::disconnect(Action act)
{
	CHECK_CONTEXT_RV(m_pContext, false);

	JQ_ASSERT( act >= Act_Leave && act <= Act_Eject );
	LONG rv = SCardDisconnect(m_handle, _act_mapping[act]);
	if (rv != SCARD_S_SUCCESS) {
		String emsg;
		emsg.sprintf("SCardDisconnect: '%s' action failed")(_act_str[act]);
		jq_emitError(emsg);
		return false;
	}
	return true;
}


bool SmartCard::reconnect(Action act, bool shared)
{
	CHECK_CONTEXT_RV(m_pContext, false);
	JQ_ASSERT( act >= Act_Leave && act <= Act_Eject );

	DWORD smode = shared ? SCARD_SHARE_SHARED : SCARD_SHARE_EXCLUSIVE;
	DWORD proto;
	LONG rv = SCardReconnect(m_handle, smode, m_preferredProto, _act_mapping[act], &proto);

	if (rv != SCARD_S_SUCCESS) {
		jq_emitError(E_SC_Reconnect);
		return false;
	}
	return true;
}

/**
 * Reads the current status of the reader connected to by smart card.
 *
 * @param sc_status
 * @return
 */
bool SmartCard::status(SmartCardStatus& sc_status) const
{
	CHECK_CONTEXT_RV(m_pContext, false);

	bool ok = true;
	char* readerName = NULL;
	DWORD readerLen = SCARD_AUTOALLOCATE;
	DWORD status;
	DWORD proto;
	BYTE  *attrPtr = NULL;
	DWORD attrCount = SCARD_AUTOALLOCATE;

	sc_status.readerName = "";
	sc_status.status     = Stat_Unknown;
	sc_status.proto      = Proto_Undefined;
	sc_status.nevents    = 0;

	LONG rv = SCardStatus(m_handle, (LPTSTR)&readerName, &readerLen,
		&status, &proto, (LPBYTE)&attrPtr, &attrCount);

	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(rv, E_SC_Status);
		ok = false;
	}

	if( rv == SCARD_S_SUCCESS ) {
		sc_status.readerName = readerName;
		sc_status.status |= status & SCARD_ABSENT ? Stat_Absent : 0;
		sc_status.status |= status & SCARD_PRESENT ? Stat_Present : 0;
		sc_status.status |= status & SCARD_SWALLOWED ? Stat_Swallowed : 0;
		sc_status.status |= status & SCARD_POWERED ? Stat_Powered : 0;
		sc_status.status |= status & SCARD_NEGOTIABLE ? Stat_Negotiable : 0;
		sc_status.status |= status & SCARD_SPECIFIC ? Stat_Specific : 0;

		// status also contains a number of events in the upper 16 bits (*pdwState & 0xFFFF0000).
		// This number of events is incremented for each card insertion or removal in the specified reader.
		// This can be used to detect a card removal/insertion between two calls to SCardStatus()
		sc_status.nevents = ((status & 0xFFFF0000) >> 16);

		sc_status.proto = _fromNativeProto(proto);

		sc_status.attrCount = attrCount;
		JQ_MEMCOPY(sc_status.attr, attrPtr, attrCount*JQ_SIZEOF(byte_t));
	}

	if( readerName ) {
		 rv = SCardFreeMemory(m_pContext->m_context, readerName);
		 if (rv != SCARD_S_SUCCESS) {
			 _jq_emitSmartCardError(rv, E_SC_FreeMem);
			 ok = false;
		 }
	}

	if( attrPtr ) {
		 rv = SCardFreeMemory(m_pContext->m_context, attrPtr);
		 if (rv != SCARD_S_SUCCESS) {
			 _jq_emitSmartCardError(rv, E_SC_FreeMem);
			 ok = false;
		 }
	}

	return ok;
}


#ifdef __COMMENT__
bool SmartCard::features(SmartCardFeatures& features) const
{
	CHECK_CONTEXT_RV(m_pContext, false);
#ifndef MAX_BUFFER_SIZE
#	define MAX_BUFFER_SIZE 264 //TODO need to wrap this macros (for PCSCLITE it defined at pcsclite.h)
#endif
	BYTE buf[MAX_BUFFER_SIZE];
	DWORD nfeatures = 0;

	features.erase();

	/* does the reader support PIN verification? */
	LONG rv = SCardControl(m_handle
		, CM_IOCTL_GET_FEATURE_REQUEST // control code
		, NULL                // command
		, 0                   // command length
		, buf                 // response buffer
		, JQ_SIZEOF(buf)      // length of response buffer
		, &nfeatures);        // length of the response

	JQ_DEBUGF( "Length of response: %ld", nfeatures );

	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(rv, E_SC_ReadFeatures);
		return false;
	}

	nfeatures /= JQ_SIZEOF(PCSC_TLV_STRUCTURE); // number of features;
	PCSC_TLV_STRUCTURE* ptr = (PCSC_TLV_STRUCTURE*)buf;

	while(nfeatures--) {
		features[ptr->tag] = ptr->value;
		ptr++;
	}
	return true;
}
#endif // __COMMENT__

/**
 * Sends a command directly to the IFD Handler (reader driver) to be processed by the reader.
 *
 * @return
 */
bool SmartCard::cmd(SmartCard::Cmd /*code*/)
{
	CHECK_CONTEXT_RV(m_pContext, false);
#if 0
	LONG rv = SCardControl( m_handle
		, // DWORD  	dwControlCode,
		, // LPCVOID  	pbSendBuffer,
		, // DWORD  	cbSendLength,
		, // LPVOID  	pbRecvBuffer,
		, // DWORD  	cbRecvLength,
		, ); //LPDWORD  	lpBytesReturned
#endif
	return false;
}

/**
 * Verify PIN
 *
 * @return
 */
bool SmartCard::verifyPIN()
{
#ifdef __COMMENT__
	CHECK_CONTEXT_RV(m_pContext, false);

	LONG rv;
	unsigned char bSendBuffer[MAX_BUFFER_SIZE];
	unsigned char bRecvBuffer[MAX_BUFFER_SIZE];
	DWORD verify_ioctl = 0;
	DWORD modify_ioctl = 0;
	DWORD length;
	PIN_VERIFY_STRUCTURE *pin_verify;

	/* does the reader support PIN verification? */
	const char* pin = "1qaZXsw2";

	rv = SCardControl(m_handle, CM_IOCTL_GET_FEATURE_REQUEST, NULL, 0,
	    bRecvBuffer, sizeof(bRecvBuffer), &length);

	/* get the number of elements instead of the complete size */
	length /= sizeof(PCSC_TLV_STRUCTURE);

	PCSC_TLV_STRUCTURE* pcsc_tlv = (PCSC_TLV_STRUCTURE*)bRecvBuffer;
	for(uint_t i = 0; i < length; i++) {
	    if (pcsc_tlv[i].tag == FEATURE_VERIFY_PIN_DIRECT)
	        verify_ioctl = pcsc_tlv[i].value;
	    if (pcsc_tlv[i].tag == FEATURE_MODIFY_PIN_DIRECT)
	        modify_ioctl = pcsc_tlv[i].value;
	}

	if (0 == verify_ioctl) {
	    JQ_ERROR("Reader does not support PIN verification");
	    return false;
	}

	pin_verify = (PIN_VERIFY_STRUCTURE *)bSendBuffer;

	/* PC/SC v2.0.2 Part 10 PIN verification data structure */
	pin_verify -> bTimerOut = 0x00;
	pin_verify -> bTimerOut2 = 0x00;
	pin_verify -> bmFormatString = 0x82;
	pin_verify -> bmPINBlockString = 0x04;
	pin_verify -> bmPINLengthFormat = 0x00;
	pin_verify -> wPINMaxExtraDigit = HOST_TO_CCID_16(0x0408); /* Min Max */
	pin_verify -> bEntryValidationCondition = 0x02; /* validation key pressed */
	pin_verify -> bNumberMessage = 0x01;
	pin_verify -> wLangId = HOST_TO_CCID_16(0x0904);
	pin_verify -> bMsgIndex = 0x00;
	pin_verify -> bTeoPrologue[0] = 0x00;
	pin_verify -> bTeoPrologue[1] = 0x00;
	pin_verify -> bTeoPrologue[2] = 0x00;
	/* pin_verify -> ulDataLength = 0x00; we don't know the size yet */

	/* APDU: 00 20 00 00 08 30 30 30 30 00 00 00 00 */
	int offset = 0;
	pin_verify -> abData[offset++] = 0x00;  /* CLA */
	pin_verify -> abData[offset++] = 0x20;  /* INS: VERIFY */
	pin_verify -> abData[offset++] = 0x00;  /* P1 */
	pin_verify -> abData[offset++] = 0x00;  /* P2 */
	pin_verify -> abData[offset++] = 0x08;  /* Lc: 8 data bytes */
	pin_verify -> abData[offset++] = 0x30;  /* '0' */
	pin_verify -> abData[offset++] = 0x30;  /* '0' */
	pin_verify -> abData[offset++] = 0x30;  /* '0' */
	pin_verify -> abData[offset++] = 0x30;  /* '0' */
	pin_verify -> abData[offset++] = 0x00;  /* '\0' */
	pin_verify -> abData[offset++] = 0x00;  /* '\0' */
	pin_verify -> abData[offset++] = 0x00;  /* '\0' */
	pin_verify -> abData[offset++] = 0x00;  /* '\0' */
	pin_verify -> ulDataLength = HOST_TO_CCID_32(offset);   /* APDU size */

	length = sizeof(PIN_VERIFY_STRUCTURE) + offset -1;
	    /* -1 because PIN_VERIFY_STRUCTURE contains the first byte of abData[] */

	printf("Enter your PIN: ");
	fflush(stdout);

	rv = SCardControl(m_handle, verify_ioctl, bSendBuffer,
	    length, bRecvBuffer, sizeof(bRecvBuffer), &length);

	if (rv != SCARD_S_SUCCESS) {
		_jq_emitSmartCardError(m_pContext, rv, _Tr("PIN verification failed"));
		return false;
	}
#endif
	return false;
}

String SmartCard::protocolToString(SmartCard::Protocol proto)
{
	switch(proto) {
		case Proto_T0: return String(_Tr("T0")); break;
		case Proto_T1: return String(_Tr("T1")); break;
		case Proto_Raw: return String(_Tr("Raw")); break;
		default: break;
	}
	return String(_T("undefined"));
}

void SmartCardStatus::dump(ostream_type& out)
{
	String text;
	text.sprintf("0x%lX")(status);
	out << _Tr("Reader: ") << readerName.c_str() << std::endl
		<< _Tr("\tStatus            : ") << text.c_str() << std::endl
		<< _Tr("\tProtocol          : ") << SmartCard::protocolToString(proto).c_str() << std::endl
		<< _Tr("\tInsertion/Removals: ") << nevents << std::endl
		<< _Tr("\tAttributes        :");

	for( int i = 0; i < attrCount; i++ ) {
		text.clear();
		text.sprintf(" %02X")(attr[i]);
		out << text.c_str();
	}

	out << std::endl;
}


void SmartCardFeatures::dump(ostream_type& out)
{
/*
	String text;
	text.sprintf("0x%lX")(status);
	out << _Tr("Reader: ") << readerName.c_str() << std::endl
		<< _Tr("\tStatus            : ") << text.c_str() << std::endl
		<< _Tr("\tProtocol          : ") << SmartCard::protocolToString(proto).c_str() << std::endl
		<< _Tr("\tInsertion/Removals: ") << nevents << std::endl
		<< _Tr("\tAttributes        :");

	for( int i = 0; i < attrCount; i++ ) {
		text.clear();
		text.sprintf(" %02X")(attr[i]);
		out << text.c_str();
	}
*/

	out << std::endl;
}


JQ_NS_END
