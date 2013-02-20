/**
 * @file   uuid.cpp
 * @author wladt
 * @date   Feb 20, 2013 3:25:35 PM
 *
 * @brief
 */

#include <cwt/uuid.hpp>

CWT_NS_BEGIN


void Uuid::uuid_copy(uuid_t &uuid, const uuid_t &other)
{
	uuid.time_low = other.time_low;
	uuid.time_mid = other.time_mid;
    uuid.time_hi_and_version = other.time_hi_and_version;
    uuid.clock_seq_hi_and_reserved = other.clock_seq_hi_and_reserved;
    uuid.clock_seq_low = other.clock_seq_low;
    uuid.node[0] = other.node[0];
    uuid.node[1] = other.node[1];
    uuid.node[2] = other.node[2];
    uuid.node[3] = other.node[3];
    uuid.node[4] = other.node[4];
    uuid.node[5] = other.node[5];
}

bool Uuid::uuid_eq(const uuid_t &uuid, const uuid_t &other)
{
	return uuid.time_low == other.time_low
			&& uuid.time_mid == other.time_mid
			&& uuid.time_hi_and_version == other.time_hi_and_version
			&& uuid.clock_seq_hi_and_reserved == other.clock_seq_hi_and_reserved
			&& uuid.clock_seq_low == other.clock_seq_low
			&& uuid.node[0] == other.node[0]
			&& uuid.node[1] == other.node[1]
			&& uuid.node[2] == other.node[2]
			&& uuid.node[3] == other.node[3]
			&& uuid.node[4] == other.node[4]
			&& uuid.node[5] == other.node[5] ? true : false;
}

/*              0-------8----3----8----3-------------*/
/* UUID format: AAAAAAAA-BBBB-CCCC-DDDD-XXXXXXXXXXXX */

bool Uuid::parse(const char *uuid_str, int len)
{
	bool ok = false;
	String s = String().fromUtf8(uuid_str, len);
	Char sep(0x002D);

	if (s.length() < 36)
		return false;

	if ( !(s[8] == sep s[13] == s[18] == s[23] == sep) )
		return false;

	m_uuid.time_low = s.substr(0, 8).toUInt(&ok, 16);
	if (!ok) return false;
	m_uuid.time_mid = s.substr(9, 4).toUShort(&ok, 16);
	if (!ok) return false;
    m_uuid.time_hi_and_version = s.substr(14, 4).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.clock_seq_hi_and_reserved = (uint8_t)s.substr(19, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.clock_seq_low = (uint8_t)s.substr(21, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[0] = (byte_t)s.substr(24, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[1] = (byte_t)s.substr(26, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[2] = (byte_t)s.substr(28, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[3] = (byte_t)s.substr(30, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[4] = (byte_t)s.substr(32, 2).toUShort(&ok, 16);
    if (!ok) return false;
    m_uuid.node[5] = (byte_t)s.substr(34, 2).toUShort(&ok, 16);
    if (!ok) return false;

	return true;
}


CWT_NS_END
