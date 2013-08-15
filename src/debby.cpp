#include "debby_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>

CWT_NS_BEGIN

Debby::Debby() : pimpl(new Debby::Impl)
{
}

bool Debby::parse(const String & str)
{
	return pimpl->parse(str);
}


bool Debby::parseFromFile(const String & path)
{
	shared_ptr<io::File> file(new io::File(path, io::Device::ReadOnly));
	/*
	if (!file->opened()) {
		Logger::error(*file);
		return false;
	}
*/
	shared_ptr<io::Device> dev = dynamic_pointer_cast<io::Device>(file);
	io::TextReader reader(dev);
	/*
	String s = reader.readAll();
	return parse(s);
		*/
	return false;
}

CWT_NS_END
