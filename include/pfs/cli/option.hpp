/*
 * option.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_OPTION_HPP__
#define __PFS_CLI_OPTION_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs { namespace cli {

class option
{
public:
	enum type_enum {
		  Boolean
		, Integer
		, Real
		, String
	};

protected:
	string _name;
	string _description;
	bool   _optional;

public:
	option (const string & name)
		: _name(name)
		, _optional(false)
	{}

	virtual ~option () {}

	virtual type_enum type () const = 0;

	void setName (const string & name)
	{
		_name = name;
	}

	void setDescription (const string & text)
	{
		_description = text;
	}

	void setOptional (bool b)
	{
		_optional = b;
	}
};


class boolean_option : public option
{
	bool   _defaultValue;
	size_t _count;

public:
	boolean_option (const string & name)
		: option(name)
		, _defaultValue(false)
		, _count(0)
	{}

	virtual type_enum type () const override
	{
		return option::Boolean;
	}

	void setDefaultValue (bool value)
	{
		_defaultValue = value;
	}
};

class integer_option : public option
{
	integral_t  _defaultValue;
	vector<integral_t> _choices;

public:
	integer_option (const string & name)
		: option(name)
		, _defaultValue(0)
	{}

	virtual type_enum type () const override
	{
		return option::Boolean;
	}

	void setDefaultValue (bool value)
	{
		_defaultValue = value;
	}
};


template <typename T>
shared_ptr<option> make_option (const string & name);

template <>
inline shared_ptr<option> make_option<bool> (const string & name)
{
	return shared_ptr<option>(new boolean_option(name));
}


}}


#endif /* __PFS_CLI_OPTION_HPP__ */
