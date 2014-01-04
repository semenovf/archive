/*
 * settings.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __CWT_SETTINGS_HPP__
#define __CWT_SETTINGS_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/string.hpp>
#include <pfs/unitype.hpp>
#include <pfs/vector.hpp>
#include <cwt/sigslot.hpp>

namespace cwt {

// MT-safe
class DLL_API settings : public has_slots<>
{
public:
	enum format {
		  JsonFormat
		, DefaultFormat = JsonFormat
	};

private:
	format _format;

	PFS_PIMPL_DECL_NOCC(settings, impl)

public:
	settings (const settings & other)
		: has_slots<>()
		, _format(DefaultFormat)
		, _pimpl(other._pimpl) {}
	settings (format f = DefaultFormat);
	~settings ();

	bool parse (const pfs::string & str);

	pfs::unitype operator [] (const pfs::string & path) { return value(path); }
	pfs::unitype value   (const pfs::string & path, const pfs::unitype & defaultValue = pfs::unitype()) const;
	bool    boolean (const pfs::string & path, bool defaultValue = false) const;
	long_t  integer (const pfs::string & path, long_t defaultValue = long_t(0)) const;
	double  number  (const pfs::string & path, double defaultValue = double(0)) const;
	pfs::string  string  (const pfs::string & path, const pfs::string & defaultValue = pfs::string()) const;
	pfs::vector<pfs::unitype>  array  (const pfs::string & path, const pfs::vector<pfs::unitype> & defaultValue = pfs::vector<pfs::unitype>()) const;

	bool contains (const pfs::string & path) const;
	//pfs::unitype::type type (const pfs::string & path) const;

/* slots */
	void set (const pfs::string & path, bool value);
	void set (const pfs::string & path, double value);
	void set (const pfs::string & path, float value);
	void set (const pfs::string & path, long_t value);
	void set (const pfs::string & path, ulong_t value);
	void set (const pfs::string & path, int_t value);
	void set (const pfs::string & path, uint_t value);
	void set (const pfs::string & path, const pfs::string & s);
	void set (const pfs::string & path, const char * latin1);
};

} // cwt

#endif /* __CWT_SETTINGS_HPP__ */
