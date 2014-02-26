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
#include <QGraphicsPixmapItem>

class Stencil : public QGraphicsPixmapItem
{
public:
	typedef pfs::map<pfs::string, cwt::dom::document> document_map_type;

private:
	static document_map_type _documents;

//	bool _isSelected;

	Q_DISABLE_COPY(Stencil)

public:
	static const char * MimeType;
	static const int IconWidth = 50;
	static const int IconHeight = 50;

public:
	Stencil (QGraphicsItem * parent = 0);
	Stencil (const pfs::string & name, QGraphicsItem * parent = 0);
	Stencil (const cwt::dom::document & doc, QGraphicsItem * parent = 0);

//	virtual pfs::string stencilIconText () const { return pfs::string(); }

	void setFromSvg (const pfs::string & svg);
	void setFromDocument (const cwt::dom::document & doc);
	//bool selected () const { return _isSelected; }
	//void select () const;

	static QPixmap icon (const pfs::string & name);
	static bool loadStencils (const pfs::string & stencilDirectory);
	static cwt::dom::document documentByName (const pfs::string & name);

protected:
	static cwt::dom::document documentFromString (const pfs::string & str);
	static cwt::dom::document documentFromFile (const pfs::string & path);
	static QPixmap toPixmap (int width, int height, const pfs::string & svg);
};

#endif /* __CWT_STENCIL_HPP__ */
