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

bool Debby::Impl::processEntity(String & code, const JsonValue::object_type & /*root*/, const JsonValue::object_type & entity)
{
	JsonValue::object_type::const_iterator it = entity.cbegin();
	JsonValue::object_type::const_iterator itEnd = entity.cend();

	while (it != itEnd) {
		String field(1, UChar('\t'));
		field.append(it.key());
		field.append(1, UChar('\n'));

		code.append(field);
		++it;
	}

	return true;
}


bool Debby::process()
{
	if (!pimpl->isObject()) {
		Logger::error("Debby's source (JSON) must be an object");
		return false;
	}

	JsonValue::object_type root = pimpl->value().object();

	JsonValue::object_type::const_iterator it = root.cbegin();
	JsonValue::object_type::const_iterator itEnd = root.cend();

	// traverse through entities
	while (it != itEnd) {

		String code = SafeFormat("struct %s {\n") % it.key();

		if (!it.value()->isObject()) {
			Logger::error(_Fr("Debby's entity '%s' must be an object") % it.key());
			return false;
		}

		// traverse through fields
		JsonValue::object_type entity = it.value()->object();
		if (!pimpl->processEntity(code, root, entity)) {
			return false;
		}

		code.append(String("};\n"));

		puts(code.c_str());
		++it;
	}

	return true;
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
