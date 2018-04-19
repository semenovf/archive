/*
 * parse.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: wladt
 */

#include "pfs/cli/cli.hpp"

namespace pfs { namespace cli {

/**
 * Командная строка состоит из массива аргументов.
 *
 * Аргументы могут быть 2-х типов:
 * - опция,
 * - свободный аргумент.
 *
 * Опция состоит из имени (optname) и значения (optarg).
 * Наличие значения опционально.
 *
 * Опция отличается от аргумента наличием префикса перед именеми.
 *
 * Имя опции отделяется от значения разделителем.
 *
 * Следует различать два типа опций:
 * - короткая опция
 * - длинная опция
 *
 * Имя короткой опции состоит из одного символа.
 *
 * @code
 * -a -B
 * /c /S
 * @endcode
 *
 * Имя длинной опции состоит из одного и более символов.
 *
 * @code
 * --a -long-option
 * /long /Option
 * @endcode
 *
 * Односимвольная длинная опция должна отличаться от короткой значением префикса.
 *
 * Короткие опции допустимо комбинировать.
 *
 * @code
 * -a -b -c -d optarg
 * /a /b /c /d:optarg
 *
 * is equivalent to
 *
 * -abcd optarg
 * /abcd:optarg
 * @endcode
 *
 */

/**
 * @brief Parses array of command line.
 *
 * @param argv List of arguments to parse
 * @return @c true if parse is successful, @c false otherwise
 */
bool cli::parse (const stringlist & argv)
{

	return false;
}

}} // pfs::cli
