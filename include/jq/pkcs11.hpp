/*
 * pkcs11.hpp
 *
 *  Created on: Sep 5, 2011
 *      Author: wladt
 */

#ifndef _JQ_PKCS11_HPP_
#define _JQ_PKCS11_HPP_

/*
#include <map>
*/
#include <vector>
#include <jq/pkcs11.h>
#include <jq/global.h>
#include <jq/dl.hpp>

JQ_NS_BEGIN

class Pkcs11Session;

class Pkcs11
{
private:
	typedef std::vector<CK_SLOT_ID> slot_list_type;

private:
	Pkcs11(Dl *mod, CK_FUNCTION_LIST* api)
		: m_module(mod)
		, m_api(api) {}

		~Pkcs11();
public:

	static Pkcs11* open(char_type *path);
	static void close(Pkcs11*);

	void version(uchar_t &major, uchar_t &minor);
	bool info( uchar_t* cryptokiMajor
		, uchar_t* cryptokiMinor
		, String* manufacturerId
		, String* libDesc
		, uchar_t* libMajor
		, uchar_t* libMinor);

	void updateSlots(bool tokenPresent = true);
	int slotsCount() const { return m_slots.size(); }
	const CK_SLOT_ID* slotAt(uint_t index) {
		return index < m_slots.size() ? &m_slots[index] : NULL; }
	bool slotInfo(uint_t index, CK_SLOT_INFO& info);

	Pkcs11Session* openSession(uint_t slotIndex) const;
	void closeSession(Pkcs11Session*) const;

private:
	Dl *m_module;
	CK_FUNCTION_LIST *m_api;
	slot_list_type m_slots;
};


class Pkcs11Session
{
	friend class Pkcs11;

	Pkcs11Session(CK_SESSION_HANDLE h) : m_session(h) {}

private:
	CK_SESSION_HANDLE m_session;
};

/*
class Pkcs11Session
{
	friend class Pkcs11;

	Pkcs11Session() {}
	~Pkcs11Session() {}

public:
	bool login();
//	bool setPin();
};
*/


JQ_NS_END


#endif /* _JQ_PKCS11_HPP_ */
