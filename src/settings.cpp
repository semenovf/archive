/*
 * settings.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "settings_p.hpp"
#include "../include/cwt/dl.hpp"

namespace cwt {

static const char * __plugin_basenames[] = {
	"cwt-settings-json"
};

class settings::impl : public settings_adapter
{};

PFS_PIMPL_DEF(settings, impl);

settings::settings (format f) : has_slots<>(), _format(f), _pimpl(new settings::impl)
{
	cwt::dl dl;
	pfs::string path = dl.buildDlFileName(_u8(__plugin_basenames[_format]));
	PFS_ASSERT(dl.pluginOpen(_u8(__plugin_basenames[_format]), path, this->_pimpl.get()));
}

settings::~settings()
{
	cwt::dl dl;
	dl.pluginClose(_u8(__plugin_basenames[_format]), this->_pimpl.get());
}

bool settings::parse(const pfs::string & str) { return _pimpl->parse(this->_pimpl.get(), str); }

void settings::set (const pfs::string & path, bool value)          { _pimpl->setBool   (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, double value)        { _pimpl->setDouble (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, float value)         { _pimpl->setFloat  (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, long_t value)        { _pimpl->setLong   (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, ulong_t value)       { _pimpl->setULong  (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, int_t value)         { _pimpl->setInt    (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, uint_t value)        { _pimpl->setUInt   (this->_pimpl.get(), path, value); }
void settings::set (const pfs::string & path, const pfs::string & s)    { _pimpl->setString (this->_pimpl.get(), path, s); }
void settings::set (const pfs::string & path, const char * latin1) { _pimpl->setLatin1 (this->_pimpl.get(), path, latin1); }

pfs::unitype settings::value (const pfs::string & path, const pfs::unitype & defaultValue) const
	{ return _pimpl->value(this->_pimpl.get(), path, defaultValue); }

bool settings::boolean (const pfs::string & path, bool defaultValue) const
	{ return _pimpl->boolean(this->_pimpl.get(), path, defaultValue); }

long_t settings::integer (const pfs::string & path, long_t defaultValue) const
	{ return _pimpl->integer(this->_pimpl.get(), path, defaultValue); }

double  settings::number (const pfs::string & path, double defaultValue) const
	{ return _pimpl->number(this->_pimpl.get(), path, defaultValue); }

pfs::string settings::string (const pfs::string & path, const pfs::string & defaultValue) const
	{ return _pimpl->string(this->_pimpl.get(), path, defaultValue); }

pfs::vector<pfs::unitype> settings::array (const pfs::string & path, const pfs::vector<pfs::unitype> & defaultValue) const
	{ return _pimpl->array(this->_pimpl.get(), path, defaultValue); }

bool settings::contains (const pfs::string & path) const
	{ return _pimpl->contains(this->_pimpl.get(), path); }

//pfs::unitype::Type settings::type (const pfs::string & path) const
//	{ return _pimpl->type(this->_pimpl.get(), path); }

} // cwt
