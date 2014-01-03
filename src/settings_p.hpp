/*
 * settings_impl.hpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "../include/cwt/settings.hpp"

namespace cwt {

class settings_adapter
{
public:
	void * _data;

public:
	settings_adapter () : _data(nullptr) {}

	bool    (* parse)   (settings_adapter * self, const pfs::string & str);
	pfs::unitype (* value)   (const settings_adapter * self, const pfs::string & path, const pfs::unitype & defaultValue);
	bool    (* boolean) (const settings_adapter * self, const pfs::string & path, bool defaultValue);
	long_t  (* integer) (const settings_adapter * self, const pfs::string & path, long_t defaultValue);
	double  (* number)  (const settings_adapter * self, const pfs::string & path, double defaultValue);
	pfs::string  (* string)  (const settings_adapter * self, const pfs::string & path, const pfs::string & defaultValue);
	pfs::vector<pfs::unitype>  (* array)  (const settings_adapter * self, const pfs::string & path, const pfs::vector<pfs::unitype> & defaultValue);

	bool (* contains)   (const settings_adapter * self, const pfs::string & path);
	//pfs::unitype::Type (* type) (const impl * self, const pfs::string & path);

	void (* setBool)   (settings_adapter * self, const pfs::string & path, bool value);
	void (* setDouble) (settings_adapter * self, const pfs::string & path, double value);
	void (* setFloat)  (settings_adapter * self, const pfs::string & path, float value);
	void (* setLong)   (settings_adapter * self, const pfs::string & path, long_t value);
	void (* setULong)  (settings_adapter * self, const pfs::string & path, ulong_t value);
	void (* setInt)    (settings_adapter * self, const pfs::string & path, int_t value);
	void (* setUInt)   (settings_adapter * self, const pfs::string & path, uint_t value);
	void (* setString) (settings_adapter * self, const pfs::string & path, const pfs::string & s);
	void (* setLatin1) (settings_adapter * self, const pfs::string & path, const char * latin1);
};

} // cwt
