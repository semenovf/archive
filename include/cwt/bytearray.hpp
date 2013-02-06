/**
 * @file   bytearray.hpp
 * @author wladt
 * @date   Jan 24, 2013 4:13:15 PM
 *
 * @brief
 */

#include <cwt/cwt.h>
#include <QtCore/QByteArray>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

class DLL_API ByteArray {
public:
	ByteArray();
	ByteArray(const char *data, int size = -1);
	ByteArray(int size, char ch);
	ByteArray(const ByteArray &other);
	~ByteArray();

	char* data();
	const char* data() const;
public:
	QByteArray __impl;
};

CWT_NS_END
