/*
 * composer.hpp
 *
 *  Created on: Aug 27, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_COMPOSER_HPP__
#define __PFS_CLI_COMPOSER_HPP__

namespace pfs { namespace cli {

class cli;
class router;
class action;
class option;

class composer
{
	enum {
		  StateInitial
		, StateAction
		, StateOption
	};

	cli *    _pcli;
	router * _prouter;
	action * _paction;
	option * _poption;
	int      _state;

public:
	composer (cli * pcli, router * prouter)
		: _pcli(pcli)
		, _prouter(prouter)
		, _state(StateInitial)
		, _paction(nullptr)
		, _poption(nullptr)
	{}

	composer & a (const string & name);
	composer & a (const char * name);
	composer & synonym (const string & name);
	composer & synonym (const char * name);
	composer & syn (const string & name) { return synonym(name); }
	composer & syn (const char * name) { return synonym(name); }
};

}} // pfs::cli

#endif /* __PFS_CLI_COMPOSER_HPP__ */
