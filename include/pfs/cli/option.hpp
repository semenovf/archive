/*
 * option.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_OPTION_HPP__
#define __PFS_CLI_OPTION_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs { namespace cli {

class option
{
public:
	enum type_enum {
		  Boolean
		, Integer
		, Number
		, String
	};

protected:
	type_enum _type;
	string    _name;
	string    _description;    // option description
	string    _default;        // default value
	                           // null means option is mandatory
	vector<string> _choices;   // the set of permissible values

public:
	option (const string & name, type_enum type = Boolean)
		: _type(type)
		, _name(name)
	{}

	~option () {}

	void setName (const string & name)
	{
		_name = name;
	}

	void setDescription (const string & text)
	{
		_description = text;
	}

	template <typename T>
	bool is () const;

	template <typename T>
	void setDefault (const T & value)
	{
		PFS_ASSERT(is<T>());
		_default = string::toString(value);
	}

	template <typename T>
	void addChoice (const T & value)
	{
		PFS_ASSERT(is<T>());
		_choices.append(string::toString(value));
	}
};

template <>
inline bool option::is<bool> () const
{
	return _type == Boolean;
}

template <>
inline bool option::is<integral_t> () const
{
	return _type == Integer;
}

template <>
inline bool option::is<real_t> () const
{
	return _type == Number;
}

template <>
inline bool option::is<string> () const
{
	return _type == String;
}

template <typename T>
option make_option (const string & name);

template <>
inline option make_option<bool> (const string & name)
{
	return option(name, option::Boolean);
}

template <>
inline option make_option<integral_t> (const string & name)
{
	return option(name, option::Integer);
}

template <>
inline option make_option<real_t> (const string & name)
{
	return option(name, option::Number);
}

template <>
inline option make_option<string> (const string & name)
{
	return option(name, option::String);
}

}}

#endif /* __PFS_CLI_OPTION_HPP__ */
