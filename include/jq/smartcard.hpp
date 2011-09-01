/*
 * SmartCard.hpp
 *
 *  Created on: Aug 25, 2011
 *      Author: wladt
 */

#ifndef _JQ_SMARTCARD_HPP_
#define _JQ_SMARTCARD_HPP_

#include <vector>
#include <map>
#include <jq/global.h>
#include <jq/errorable.hpp>

#ifdef _WIN32
#	error "Not implemented yet"
#else
#	include <winscard.h>
#endif

namespace jq {

class SmartCard;
struct SmartCardStatus;
class SmartCardFeatures;

class SmartCardContext : public Errorable
{
	friend class SmartCard;
public:
	typedef std::vector<String>      reader_list_type;

public:
	SmartCardContext();
	~SmartCardContext();

	bool isValid();

	/*void updateReaderGroups();*/
	void updateReaders();
	const std::vector<String>& readers() const { return m_readers; }

	int readersCount() const { return m_readers.size(); }
	const String& readerAt(uint index) {
		return index < m_readers.size() ? m_readers[index] : String::Empty; }

	bool waitAnyReader(ulong millis, bool *timedout = NULL);
	bool cancel();

private:
	SCARDCONTEXT m_context;
	reader_list_type m_readers;
/*	std::vector<String> m_readerGroups;*/
};


class SmartCard
{
public:
	typedef ulong Cmd;
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
		, m_preferredProto(Proto_Undefined) { }

	~SmartCard() { disconnect(); }

	bool connect(ShareMode smode, Protocol preferred);
	bool disconnect()   { return disconnect(Act_Leave); }
	bool reset()   { return disconnect(Act_Reset); }
	bool unpower() { return disconnect(Act_Unpower); }
	bool eject()   { return disconnect(Act_Eject); }
	bool status(SmartCardStatus& status) const;
	bool features(SmartCardFeatures& features) const;

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
	uint                m_preferredProto; // for internal use only
};


struct SmartCardStatus
{
	String              readerName;
	int                 status;
	int                 nevents;
	SmartCard::Protocol proto;
	byte_t              attr[MAX_ATR_SIZE];
	int                 attrCount;

	void dump(ostream_type& out);
};

class SmartCardFeatures
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

} /* namespace jq */

#endif /* _JQ_SMARTCARD_HPP_ */
