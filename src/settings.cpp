/*
 * settings.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "settings_p.hpp"
#include "pfs/dl.hpp"

namespace pfs {

static const pfs::string __plugin_basenames[] = {
	_u8("pfs-settings-json")
};

class settings_impl : public settings_adapter
{
	settings::format _format;
public:
	settings_impl (settings::format f)
		: settings_adapter()
		, _format(f) {}

	settings::format format () const { return _format; }
};

settings::settings (format f)
	: base_class(new settings_impl(f))
{
	pfs::dl dl;
	pfs::string path = dl.buildDlFileName(__plugin_basenames[f]);
	PFS_ASSERT_X(
			dl.pluginOpen(__plugin_basenames[f], path, base_class::cast())
			, dl.lastErrorText().c_str());
}

settings::~settings()
{
	pfs::dl dl;
	impl_class * d = base_class::cast();
	dl.pluginClose(__plugin_basenames[d->format()], d);
}

bool settings::parse(const pfs::string & str)
{
	impl_class * d = base_class::cast();
	return d->parse(d, str);
}

settings::property
settings::value (const pfs::string & path
		, const property & defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->value(d, path, defaultValue);
}

bool
settings::booleanValue (const pfs::string & path
		, bool defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->booleanValue(d, path, defaultValue);
}

integral_t
settings::integralValue (const pfs::string & path
		, integral_t defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->integralValue(d, path, defaultValue);
}

real_t
settings::realValue (const pfs::string & path
		, real_t defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->realValue(d, path, defaultValue);
}

pfs::string
settings::stringValue (const pfs::string & path
		, const pfs::string & defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->stringValue(d, path, defaultValue);
}

pfs::vector<settings::property>
settings::arrayValue (const pfs::string & path
		, const pfs::vector<property> & defaultValue) const
{
	const impl_class * d = base_class::cast();
	return d->arrayValue(d, path, defaultValue);
}

bool settings::contains (const pfs::string & path) const
{
	const impl_class * d = base_class::cast();
	return d->contains(d, path);
}

void settings::set (const pfs::string & path, bool value)
{
	impl_class * d = base_class::cast();
	d->setBool(d, path, value);
}

void settings::set (const pfs::string & path, integral_t value)
{
	impl_class * d = base_class::cast();
	d->setIntegral(d, path, value);
}

void settings::set (const pfs::string & path, uintegral_t value)
{
	impl_class * d = base_class::cast();
	d->setUIntegral(d, path, value);
}

void settings::set (const pfs::string & path, real_t value)
{
	impl_class * d = base_class::cast();
	d->setReal(d, path, value);
}

void settings::set (const pfs::string & path, const pfs::string & value)
{
	impl_class * d = base_class::cast();
	d->setString(d, path, value);
}

} // pfs
