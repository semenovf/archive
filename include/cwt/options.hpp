/*
 * options.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __CWT_OPTIONS_HPP__
#define __CWT_OPTIONS_HPP__

#include <cwt/errorable.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

struct Options
{
	enum TypeEnum {
		  ShortOpt
		, LongOpt
		, LongOptWithArg
		, SingleDash
		, DoubleDash
		, Argument
	};

	enum ArgTypeEnum {
		  BoolArg
		, NumberArg
		, RealArg
		, StringArg
	};

	char        *longname;  /* long option or NULL */
	char        *shortname; /* short option or 0 */
	ArgTypeEnum  argType;   /* option's argument type (see ArgTypeEnum) */
	String       xpath;     /* path to appropriate settings node  */
	String       desc;      /* option's description (for automatically generated usage) */
//	bool        (*validator)(const void*);  /* validation function for option argument */
};

class DLL_API_EXPORT OptionsReader : public Errorable
{
	static const char* DEFAULT_SHORT_PREFIX;     // "-"
	static const char* DEFAULT_LONG_PREFIX;      // "--"
	static const char* DEFAULT_OPTARG_SEPARATOR; // "="

	CWT_DENY_COPY(OptionsReader);
public:
	OptionsReader();
	OptionsReader(const char *short_prefix, const char *long_prefix, const char *separator);

	bool parse(int argc, char *argv[]);

private:
	ByteArray m_shortPrefix;
	ByteArray m_longPrefix;
	ByteArray m_separator;
};

OptionsReader::OptionsReader()
	: m_shortPrefix(DEFAULT_SHORT_PREFIX)
	, m_longPrefix(DEFAULT_LONG_PREFIX)
	, m_separator(DEFAULT_OPTARG_SEPARATOR)
{}

OptionsReader::OptionsReader(const char *short_prefix, const char *long_prefix, const char *separator)
	: m_shortPrefix(short_prefix)
	, m_longPrefix(long_prefix)
	, m_separator(separator)
{}

CWT_NS_END

#endif /* __CWT_OPTIONS_HPP__ */
