/**
 * @file StencilListView.hpp
 * @author wladt
 * @date Feb 26, 2014
 */

#ifndef __STENCILLISTVIEW_HPP__
#define __STENCILLISTVIEW_HPP__

#include <pfs.hpp>
#include <QListView>

class StencilListModel;

class StencilListView : public QListView
{
	StencilListModel * _model;

	Q_OBJECT

public:
	 explicit StencilListView (QWidget * parent = 0)
	 	 : QListView(parent) {}

	 void setViewMode(ViewMode vm);
public:
	 // Whether to access the filtered or unfiltered view
	 //enum AccessMode { FilteredAccess, UnfilteredAccess };

	 //explicit WidgetBoxCategoryListView(QDesignerFormEditorInterface *core, QWidget *parent = 0);


void dropWidgets(const QList<QDesignerDnDItemInterface*> &item_list);

using QListView::contentsSize;

// These methods operate on the filtered/unfiltered model according to accessmode
int count(AccessMode am) const;
QDesignerWidgetBoxInterface::Widget widgetAt(AccessMode am, const QModelIndex &index) const;
QDesignerWidgetBoxInterface::Widget widgetAt(AccessMode am, int row) const;
void removeRow(AccessMode am, int row);
void setCurrentItem(AccessMode am, int row);

// These methods operate on the unfiltered model and are used for serialization
void addWidget(const QDesignerWidgetBoxInterface::Widget &widget, const QIcon &icon, bool editable);
bool containsWidget(const QString &name);
QDesignerWidgetBoxInterface::Category category() const;
bool removeCustomWidgets();

// Helper: Ensure a <ui> tag in the case of empty XML
static QString widgetDomXml(const QDesignerWidgetBoxInterface::Widget &widget);

signals:
void scratchPadChanged();
void pressed(const QString &name, const QString &xml, const QPoint &globalPos);
void itemRemoved();
void lastItemRemoved();

public slots:
void filter(const QRegExp &re);

private slots:
void slotPressed(const QModelIndex &index);
void removeCurrentItem();
void editCurrentItem();

private:
int mapRowToSource(int filterRow) const;
QSortFilterProxyModel *m_proxyModel;
WidgetBoxCategoryModel *m_model;
}

#endif /* __STENCILLISTVIEW_HPP__ */
