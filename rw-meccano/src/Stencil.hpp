/**
 * @file Stencil.hpp
 * @author wladt
 * @date Feb 25, 2014
 */

#ifndef __CWT_STENCIL_HPP__
#define __CWT_STENCIL_HPP__

#include <pfs/map.hpp>
#include <pfs/string.hpp>
#include <cwt/xml/dom.hpp>
#include <QPixmap>

class QGraphicsSvgItem;

class Stencil/* : public QGraphicsPixmapItem*/
{
public:
	typedef pfs::map<pfs::string, cwt::dom::document> document_map_type;

private: // static
	static document_map_type _documents;

private:
	pfs::string _name;

public:
//	static const char * MimeType;
	static const int IconWidth = 50;
	static const int IconHeight = 50;

public:
	Stencil (const pfs::string & name);
	Stencil (const Stencil & other);
	Stencil & operator = (const Stencil & other);

	const pfs::string & name () const { return _name; }
	void setName (const pfs::string name) { _name = name; }

	QPixmap toPixmap ();
	QPixmap toPixmap (int scale);
	QPixmap toPixmap (int width, int height);

	QPixmap toIcon ();

//	void setFromSvg (const pfs::string & svg);
//	void setFromDocument (const cwt::dom::document & doc);

	QGraphicsSvgItem * createGraphicsSvgItem ();

public: // static
	//static QPixmap icon (const pfs::string & name);
	static bool loadStencils (const pfs::string & stencilDirectory);


protected: // static
	static cwt::dom::document documentByName (const pfs::string & name);
	static cwt::dom::document documentFromString (const pfs::string & str);
	static cwt::dom::document documentFromFile (const pfs::string & path);
};

#endif /* __CWT_STENCIL_HPP__ */
