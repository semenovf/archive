#include "debby_p.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>

CWT_NS_BEGIN

bool Debby::process()
{
	return pimpl->parseAll();
}

bool Debby::parseFromFile(const String & path)
{
	shared_ptr<io::File> file(new io::File(path, io::Device::ReadOnly));
	if (!file->opened()) {
		return false;
	}

	shared_ptr<io::Device> dev = dynamic_pointer_cast<io::Device>(file);
	io::TextReader reader(dev);
	String s = reader.readAll();

	if (!parse(s)) {
		Logger::error(_Fr("Bad debby file: %s") % path);
		return false;
	}

	return process();
}

CWT_NS_END
