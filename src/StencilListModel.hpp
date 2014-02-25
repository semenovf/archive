/**
 * @file StencilList.hpp
 * @author wladt
 * @date Feb 25, 2014
 */

#ifndef __STENCILLISTMODEL_HPP__
#define __STENCILLISTMODEL_HPP__

#include <pfs.hpp>
#include <QAbstractListModel>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class StencilListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    StencilListModel (QObject * parent = 0);

    virtual int rowCount (const QModelIndex & parent) const override;
    virtual QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags (const QModelIndex & index) const override;
    virtual bool removeRows (int row, int count, const QModelIndex & parent) override;
    //bool dropMimeData (const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
    virtual QMimeData * mimeData (const QModelIndexList & indexes) const override;

    // list of MIME types that can be used to describe a list of model indexes.
    virtual QStringList mimeTypes () const override;

    Qt::DropActions supportedDropActions () const;

    void addStencil (const QPixmap & pixmap);
    void populateStencils ();

private:
//    QList<QPoint>  _locations;
    QList<QPixmap> _stencils;
};


#endif /* __STENCILLISTMODEL_HPP__ */
