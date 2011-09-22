/*
 * smartcard.hpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#ifndef _JQ_SMARTCARD_HPP_
#define _JQ_SMARTCARD_HPP_

#include <vector>
#include <map>
#include <jq/global.h>
#include <jq/string.hpp>

#ifdef JQ_WIN32
#	include <winscard.h>
#	ifndef MAX_ATR_SIZE
#		define MAX_ATR_SIZE 36 // =36 - for windows, =33 as defined in pcsclite.h
#	endif
#else
#	include <winscard.h>
#endif

JQ_NS_BEGIN

struct SmartCardStatus;
class SmartCard;
class SmartCardFeatures;

class DLL_API SmartCardContext
{
	friend class SmartCard;

private:
	typedef std::vector<String> reader_list_type;

public:
	SmartCardContext();
	~SmartCardContext();

	bool isValid();

	/*void updateReaderGroups();*/
	void updateReaders();
	//const reader_list_type& readers() const { return m_readers; }

	int readersCount() const { return m_readers.size(); }
	const String& readerAt(uint_t index) {
		return index < m_readers.size() ? m_readers[index] : String::emptyString(); }

	bool waitAnyReader(ulong_t millis, bool *timedout = NULL);
	bool cancel();

	static bool serviceAvailable(const void* data=NULL);

private:
	SCARDCONTEXT m_context;
	reader_list_type m_readers;
/*	std::vector<String> m_readerGroups;*/
};


class DLL_API SmartCard
{
public:
	typedef ulong_t Cmd;
	//static const Cmd_;

	typedef enum {
		SM_Exclusive = 1,
		SM_Shared,
		SM_Direct
	} ShareMode;

	typedef enum {
		Proto_Undefined,
		Proto_T0,
		Proto_T1,
		Proto_Any, // Proto_T0 | Proto_T1
		Proto_Raw
	} Protocol;

	typedef enum {
		Act_Leave,
		Act_Reset,
		Act_Unpower,
		Act_Eject
	} Action;

	typedef enum {
		Stat_Unknown    = 0x0001,
		Stat_Absent     = 0x0002,
		Stat_Present    = 0x0004,
		Stat_Swallowed  = 0x0008,
		Stat_Powered    = 0x0010,
		Stat_Negotiable = 0x0020,
		Stat_Specific   = 0x0040
	} Status;

public:
	SmartCard( SmartCardContext& context, const String& reader )
		: m_pContext( &context )
		, m_readerName(reader)
		, m_preferredProto(Proto_Undefined)
	{ }

	~SmartCard() { disconnect(); }

	bool connect(ShareMode smode, Protocol preferred);
	bool disconnect()   { return disconnect(Act_Leave); }
	bool reset()   { return disconnect(Act_Reset); }
	bool unpower() { return disconnect(Act_Unpower); }
	bool eject()   { return disconnect(Act_Eject); }
	bool status(SmartCardStatus& status) const;
#ifdef __NOT_IMPLEMETED_YET__
	bool features(SmartCardFeatures& features) const;
#endif

	bool verifyPIN();

	static String protocolToString(SmartCard::Protocol proto);

protected:
	bool cmd(Cmd code);
	bool disconnect(Action act);
	bool reconnect(Action act, bool shared = true);

private:
	SmartCardContext*   m_pContext;
	String              m_readerName;
	SCARDHANDLE         m_handle;	      // card handle
	uint_t              m_preferredProto; // for internal use only
};


struct DLL_API SmartCardStatus
{
	String              readerName;
	int                 status;
	int                 nevents;
	SmartCard::Protocol proto;
	byte_t              attr[MAX_ATR_SIZE];
	int                 attrCount;

	void dump(ostream_type& out);
};

class DLL_API SmartCardFeatures
{
	friend class SmartCard;
	typedef std::map<uint8_t, uint32_t> feature_list_type;

	void erase() { m_features.erase(m_features.begin(), m_features.end()); }
	uint32_t& operator[](const uint8_t& k) { return m_features.operator[](k); }

public:
	SmartCardFeatures() {}
	~SmartCardFeatures() {}
	void dump(ostream_type& out);

private:
	feature_list_type m_features;
};

JQ_NS_END

#endif /* _JQ_SMARTCARD_HPP_ */
