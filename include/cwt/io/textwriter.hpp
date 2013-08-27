/*
 * textwriter.hpp
 *
 *  Created on: Aug 13, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_TEXTWRITER_HPP__
#define __CWT_IO_TEXTWRITER_HPP__

#include <cwt/io/device.hpp>
#include <cwt/io/writer.hpp>
#include <cwt/io/utf8codec.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API TextWriter : public Writer<Device, Utf8Encoder>
{
private:
	typedef Writer<Device, Utf8Encoder> base_class;

	CWT_DENY_COPY(TextWriter);

/*
public:
	typedef typename base_class::string_type string_type;
*/

public:
	TextWriter(shared_ptr<Device> dev)
		: base_class(dev, make_shared<Utf8Encoder>())
		{}
	TextWriter(shared_ptr<Device> dev, shared_ptr<Utf8Encoder> encoder)
		: base_class(dev, encoder)
		{}

	Device * device() const { return consumer(); }
	ssize_t write (const String & s) { return write(s.data(), s.size()); }
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_TEXTWRITER_HPP__ */
