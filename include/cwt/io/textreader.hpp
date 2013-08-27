/*
 * textreader.hpp
 *
 *  Created on: Aug 6, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_TEXTREADER_HPP__
#define __CWT_IO_TEXTREADER_HPP__

#include <cwt/io/device.hpp>
#include <cwt/io/reader.hpp>
#include <cwt/io/utf8codec.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API TextReader : public Reader<Device, Utf8Decoder>
{
private:
	typedef Reader<Device, Utf8Decoder> base_class;

	CWT_DENY_COPY(TextReader);

public:
	TextReader(shared_ptr<Device> dev, shared_ptr<Utf8Decoder> decoder)
		: base_class(dev, decoder) {}

	TextReader(shared_ptr<Device> dev)
		: base_class(dev, shared_ptr<Utf8Decoder>(new Utf8NullDecoder)) {}

	Device * device() const { return this->producer(); }

	Utf8String readAll()
	{
		return read(device()->available());
	}
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_TEXTREADER_HPP__ */
