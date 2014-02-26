/**
 * @file StencilListView.cpp
 * @author wladt
 * @date Feb 26, 2014
 */

#include "StencilListView.hpp"
#include <QtGui>

void StencilListView::startDrag (Qt::DropActions supportedActions)
{
	PFS_DEBUG(qDebug() << "StencilListView::startDrag");
	QListView::startDrag(supportedActions);
}
