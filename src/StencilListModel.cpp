/**
 * @file StencilListModel.cpp
 * @author wladt
 * @date Feb 25, 2014
 */

#include "StencilListModel.hpp"
#include "Stencil.hpp"
#include "TrafficLights.hpp"

#include <QtGui>


StencilListModel::StencilListModel (QObject * parent)
    : QAbstractListModel(parent)
{}

QVariant StencilListModel::data (const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
        return QIcon(_stencils.value(index.row()).scaled(Stencil::Size, Stencil::Size,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else if (role == Qt::UserRole)
        return _stencils.value(index.row());

    return QVariant();
}

Qt::ItemFlags StencilListModel::flags (const QModelIndex & index) const
{
    if (index.isValid())
        return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

    return Qt::ItemIsDropEnabled;
}

bool StencilListModel::removeRows (int row, int count, const QModelIndex & parent)
{
    if (parent.isValid())
        return false;

    if (row >= _stencils.size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, _stencils.size() - 1);

    beginRemoveRows(parent, beginRow, endRow);

    while (beginRow <= endRow) {
        _stencils.removeAt(beginRow);
        ++beginRow;
    }

    endRemoveRows();
    return true;
}

QStringList StencilListModel::mimeTypes () const
{
    QStringList types;
    types << Stencil::MimeType;
    return types;
}

QMimeData * StencilListModel::mimeData (const QModelIndexList & indexes) const
{
    QMimeData * mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(& encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QPixmap pixmap = qvariant_cast<QPixmap>(data(index, Qt::UserRole));
            stream << pixmap /*<< location*/;
        }
    }

    mimeData->setData(Stencil::MimeType, encodedData);
    return mimeData;
}

/*bool StencilListModel::dropMimeData (const QMimeData * data, Qt::DropAction action,
                               int row, int column, const QModelIndex & parent)
{
    if (!data->hasFormat(__mime_type))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    if (column > 0)
        return false;

    int endRow;

    if (!parent.isValid()) {
        if (row < 0)
            endRow = pixmaps.size();
        else
            endRow = qMin(row, pixmaps.size());
    } else
        endRow = parent.row();

    QByteArray encodedData = data->data(__mime_type);
    QDataStream stream(& encodedData, QIODevice::ReadOnly);

    while (!stream.atEnd()) {
        QPixmap pixmap;
        QPoint location;
        stream >> pixmap >> location;

        beginInsertRows(QModelIndex(), endRow, endRow);
        pixmaps.insert(endRow, pixmap);
        locations.insert(endRow, location);
        endInsertRows();

        ++endRow;
    }

    return true;
}*/

int StencilListModel::rowCount (const QModelIndex & parent) const
{
    if (parent.isValid())
        return 0;
    else
        return _stencils.size();
}

Qt::DropActions StencilListModel::supportedDropActions () const
{
    return Qt::CopyAction | Qt::MoveAction;
}


void StencilListModel::addStencil (const QPixmap & pixmap/*, const QPoint & location*/)
{
    int row = _stencils.size();

    beginInsertRows(QModelIndex(), row, row);
    _stencils.insert(row, pixmap);
    endInsertRows();
}

void StencilListModel::populateStencils ()
{
	if (_stencils.size()) {
		beginRemoveRows(QModelIndex(), 0, _stencils.size());
		_stencils.clear();
		endRemoveRows();
	}

    TrafficLights trafficLights;
    addStencil(trafficLights.toPixmap());
}
