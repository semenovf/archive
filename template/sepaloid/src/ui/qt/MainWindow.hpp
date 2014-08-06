/**
 * @file MainWindow.hpp
 * @author
 * @date
 *
 * @brief
 */

#ifndef __UI_QT_MAINWINDOW_HPP__
#define __UI_QT_MAINWINDOW_HPP__

#include "petaloid.hpp"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
	QToolBar          * _toolBar;

	Q_OBJECT

public Q_SLOTS:

private:
	void setupMenuBar ();
	void setupToolBar ();
	void setupStatusBar ();
	void setupWidgets ();

public:
	MainWindow();
	~MainWindow() {}
};

#endif /* __UI_QT_MAINWINDOW_HPP__ */
