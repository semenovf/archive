/**
 * @file StencilListView.cpp
 * @author wladt
 * @date Feb 26, 2014
 */

#include "StencilListView.hpp"
#include <QtGui>


QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class Stencil;

class StencilListModel : public QAbstractListModel
{
//    Q_OBJECT
	QListView::ViewMode _viewMode;
	QList<QString>      _items;

//    typedef QList<WidgetBoxCategoryEntry> WidgetBoxCategoryEntrys;

//    QRegExp m_classNameRegExp;
//    QDesignerFormEditorInterface *m_core;
//    WidgetBoxCategoryEntrys m_items;
//    QListView::ViewMode m_viewMode;

public:
    StencilListModel (QObject * parent = 0);

    virtual QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const override;
    virtual int rowCount (const QModelIndex & parent = QModelIndex()) const override;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex()) override;
    virtual Qt::ItemFlags flags (const QModelIndex & index) const override;

    // The model returns no text in icon mode, so, it also needs to know it
    QListView::ViewMode viewMode() const { return _viewMode; }
    void setViewMode(QListView::ViewMode vm)
    {
        if (_viewMode == vm)
            return;
        const bool empty = _items.isEmpty();
        if (!empty)
            beginResetModel();
        _viewMode = vm;
        if (!empty)
            endResetModel();
    }

    void addWidget(const QDesignerWidgetBoxInterface::Widget &widget, const QIcon &icon, bool editable);

    QDesignerWidgetBoxInterface::Widget widgetAt(const QModelIndex & index) const;
    QDesignerWidgetBoxInterface::Widget widgetAt(int row) const;

    int indexOfWidget(const QString &name);

//    QDesignerWidgetBoxInterface::Category category() const;
//    bool removeCustomWidgets();

//    virtual QMimeData * mimeData (const QModelIndexList & indexes) const override;
    // list of MIME types that can be used to describe a list of model indexes.
//    virtual QStringList mimeTypes () const override;

//    Qt::DropActions supportedDropActions () const;

//    void populateStencils ();
};


StencilListModel::StencilListModel (QObject * parent)
    : QAbstractListModel(parent)
{}

QVariant StencilListModel::data (const QModelIndex & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QString stencilName = _stencils.value(index.row());

    switch(role) {
    case Qt::DecorationRole: {
    	Stencil stencil(qcast(stencilName));
    	return QIcon(stencil.toIcon().scaled(Stencil::IconWidth, Stencil::IconHeight,
        		Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    	break;

    case Qt::DisplayRole:
    	return stencilName;

    case Qt::ToolTipRole:
    	return stencilName;

    case Qt::UserRole:
    	return stencilName;
    }

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
    types << "text/plain"; //Stencil::MimeType;
    return types;
}

QMimeData * StencilListModel::mimeData (const QModelIndexList & indexes) const
{
	QMimeData * mimeData = new QMimeData();
	QStringList stencilNames;

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
        	stencilNames << qvariant_cast<QString>(data(index, Qt::UserRole));
        }
    }

    mimeData->setText(stencilNames.join(";"));
    return mimeData;

/*
    QMimeData * mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(& encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QPixmap pixmap = qvariant_cast<QPixmap>(data(index, Qt::UserRole));
            stream << pixmap << location;
        }
    }

    mimeData->setData(Stencil::MimeType, encodedData);
    return mimeData;
*/
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
    return Qt::IgnoreAction; //Qt::CopyAction | Qt::MoveAction;
}

void StencilListModel::populateStencils ()
{
	if (_stencils.size()) {
		beginRemoveRows(QModelIndex(), 0, _stencils.size());
		_stencils.clear();
		endRemoveRows();
	}

    int row = _stencils.size();

    beginInsertRows(QModelIndex(), row, row);

    //PFS_ASSERT(!Stencil::documentByName(_l1("station-name")).isNull());
    _stencils.insert(row++, QString("station-name"));
    _stencils.insert(row++, QString("block-section"));
    _stencils.insert(row++, QString("direction-arrow"));
    _stencils.insert(row++, QString("trafficlight-dwarf"));
    _stencils.insert(row++, QString("trafficlight-mast"));

    qDebug() << "columnCount=" << columnCount(QModelIndex());
    qDebug() << "rowCount=" << rowCount(QModelIndex());

    endInsertRows();
}


//-------------- StencilListView -------------------

StencilListView::StencilListView (QWidget * parent)
	 : QListView(parent)
	, _model(new StencilListModel)
{
    setWordWrap(false);
    setDragEnabled(true);
    setViewMode(QListView::ListMode);
    setIconSize(QSize(Stencil::IconWidth, Stencil::IconHeight));
    setGridSize(QSize(Stencil::IconWidth + 10, Stencil::IconHeight + 10));
//    _stencilListView->setSpacing(10);
//    _stencilListView->setMovement(QListView::Free);
    //_stencilListView->setFlow(QListView::TopToBottom);
    setAcceptDrops(false);
    setDropIndicatorShown(true);

    _model->populateStencils();
    setModel(_model);


}
