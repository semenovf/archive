/**
 * @file Stencil.hpp
 * @author wladt
 * @date Feb 25, 2014
 */

#ifndef __CWT_STENCIL_HPP__
#define __CWT_STENCIL_HPP__

#include <pfs/map.hpp>
#include <pfs/string.hpp>
#include <QPixmap>

class Stencil
{
public:
	typedef pfs::map<pfs::string, Stencil> map_type;

private:
	pfs::string _filepath;
	static map_type _stencils;

public:
	static const char * MimeType;
	static const int Size = 50;

public:
	Stencil (const pfs::string & filepath = pfs::string()) : _filepath(filepath) {}
	QPixmap toPixmap () const;

	static const map_type & stencilMap () { return _stencils; }
	static bool loadStencils (const pfs::string & stencilDirecory);
	static Stencil getStencilByName (const pfs::string & name);
};

#endif /* __CWT_STENCIL_HPP__ */
