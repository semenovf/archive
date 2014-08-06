/**
 * @file LogWidget.cpp
 * @author
 * @date
 */


#include "LogWidget.hpp"
#include "petaloid.hpp"
#include "qcast.hpp"
#include <QStringList>
#include <QHeaderView>
#include <QTableWidgetItem>

LogWidget::LogWidget (QWidget * parent) : QTableWidget(parent), _rowLimit(100)
{
	setColumnCount(2);
	setRowCount(0);

	_errorFgColor = Qt::red;
	_errorBgColor = Qt::transparent;

	QStringList hHeader;
	hHeader	<< QString::fromUtf8(_Tr("Время"))
		<< QString::fromUtf8(_Tr("Событие"));

	setHorizontalHeaderLabels (hHeader);
	verticalHeader()->hide();
	//verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setStretchLastSection(true);

	connect(& __PETALOID->qt, SIGNAL(emitInfo  (pfs::datetime, pfs::string)), this, SLOT(info  (pfs::datetime, pfs::string)));
	connect(& __PETALOID->qt, SIGNAL(emitDebug (pfs::datetime, pfs::string)), this, SLOT(debug (pfs::datetime, pfs::string)));
	connect(& __PETALOID->qt, SIGNAL(emitWarn  (pfs::datetime, pfs::string)), this, SLOT(warn  (pfs::datetime, pfs::string)));
	connect(& __PETALOID->qt, SIGNAL(emitError (pfs::datetime, pfs::string)), this, SLOT(error (pfs::datetime, pfs::string)));
}

void LogWidget::log (const pfs::string & pixname, QColor fg, QColor bg, pfs::datetime dt, pfs::string msg)
{
	if (rowCount() >= _rowLimit) {
		this->removeRow(0);
	}

	int row = rowCount();
	QTableWidgetItem * i0 = new QTableWidgetItem(qcast(dt.toString()));
	i0->setIcon(QIcon(QPixmap(qcast(pixname))));
	QTableWidgetItem * i1 = new QTableWidgetItem(qcast(msg));

	if (bg.isValid()) {
		i0->setBackground(bg);
		i1->setBackground(bg);
	}

	if (fg.isValid()) {
		i0->setForeground(fg);
		i1->setForeground(fg);
	}

	this->insertRow(row);
	this->setItem(row, 0, i0);
	this->setItem(row, 1, i1);
	this->scrollToItem(i0, QAbstractItemView::PositionAtBottom);
	this->resizeColumnToContents(0);
}
