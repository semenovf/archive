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
	io::File file(path, io::Device::ReadOnly);
	if (!file.opened()) {
		return false;
	}

	io::TextReader reader(file);
	String s = reader.readAll();

	if (!parse(s)) {
		Logger::error(_Fr("Bad debby file: %s") % path);
		return false;
	}

	return process();
}

CWT_NS_END
