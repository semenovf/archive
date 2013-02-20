/**
 * @file   uuid.hpp
 * @author wladt
 * @date   Feb 20, 2013 3:25:34 PM
 *
 * @brief
 */

#ifndef __CWT_UUID_HPP__
#define __CWT_UUID_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>

CWT_NS_BEGIN

class Uuid {
public:
	Uuid() { static uuid_t null_uuid = CWT_NULL_UUID; uuid_copy(m_uuid, null_uuid); }
	Uuid(const Uuid &uuid) { uuid_copy(m_uuid, uuid.m_uuid); }
	Uuid(const uuid_t &uuid) { uuid_copy(m_uuid, uuid); }

	bool isNull() const;
	bool parse(const char *uuid_str, int len = -1);
	String toString(const uuid_t &uuid) const;
	const uuid_t& uuid() const { return m_uuid; }

	Uuid& operator = (const Uuid &uuid)   { uuid_copy(m_uuid, uuid.m_uuid); return *this; }
	Uuid& operator = (const uuid_t &uuid) { uuid_copy(m_uuid, uuid); return *this; }

	friend bool	operator==(const Uuid &uuid1, const Uuid &uuid2) { return Uuid::uuid_eq(uuid1.m_uuid, uuid2.m_uuid); }
	friend bool	operator==(const Uuid &uuid1, const uuid_t &uuid2) { return Uuid::uuid_eq(uuid1.m_uuid, uuid2); }

public:
	static void uuid_copy(uuid_t &uuid, const uuid_t &other);
	static bool uuid_eq(const uuid_t &uuid, const uuid_t &other);
private:
	uuid_t m_uuid;
};

CWT_NS_END

#endif /* __CWT_UUID_HPP__ */
