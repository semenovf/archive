/**
 * @file StencilListView.hpp
 * @author wladt
 * @date Feb 26, 2014
 */

#ifndef __STENCILLISTVIEW_HPP__
#define __STENCILLISTVIEW_HPP__

#include <pfs.hpp>
#include <QListView>

class StencilListView : public QListView
{
public:
	 explicit StencilListView (QWidget * parent = 0)
	 	 : QListView(parent) {}

protected:
	 virtual void startDrag (Qt::DropActions supportedActions) override;
};

#endif /* __STENCILLISTVIEW_HPP__ */
