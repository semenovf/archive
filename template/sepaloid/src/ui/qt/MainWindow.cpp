/**
 * @file   MainWindow.cpp
 * @author wladt
 * @date   Feb 13, 2014
 *
 * @brief
 */

#include "MainWindow.hpp"
#include "qcast.hpp"
#include <cwt/logger.hpp>
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
//#include <QStringList>

void MainWindow::setupMenuBar ()
{
	QMenuBar * menuBar = this->menuBar();
	QMenu * fileMenu  = menuBar->addMenu(QString().fromUtf8(_Tr("File")));
	QMenu * editMenu  = menuBar->addMenu(QString().fromUtf8(_Tr("Edit")));
	QMenu * helpMenu  = menuBar->addMenu(QString().fromUtf8(_Tr("Help")));

	QAction * actOpen = new QAction(this);
	actOpen->setText(QString().fromUtf8(_Tr("Open ...")));

	/* File menu */
	QAction * separator = new QAction(this);
	separator->setSeparator(true);

	QAction * actQuit = new QAction(this);
	actQuit->setText(QString().fromUtf8(_Tr("Quit")));

	fileMenu->addAction(actOpen);
	fileMenu->addAction(separator);
	fileMenu->addAction(actQuit);

	/* Edit menu */
	QAction * actPrefsDialog = new QAction(this);
	actPrefsDialog->setText(QString().fromUtf8(_Tr("Preferences...")));
	editMenu->addAction(actPrefsDialog);

	/* Help menu */
	QAction * actAboutQt = new QAction(this);
	actAboutQt->setText(QString().fromUtf8(_Tr("About &Qt...")));

	helpMenu->addAction(actAboutQt);

//	connect(actOpen, SIGNAL(triggered()), this, SLOT(openDialog()));
	connect(actAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));
	connect(actQuit, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::setupToolBar ()
{
//	_toolBar = new QToolBar(this);
//	addToolBar(_toolBar);
//	_toolBar->setAllowedAreas(Qt::TopToolBarArea);
//	_toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
//
//	QAction * actZoomIn = new QAction(this);
//	QAction * actZoomOut = new QAction(this);
//	QAction * actZoomExtend = new QAction(this);
//
//	actZoomIn->setIcon(QIcon(QString().fromUtf8(":/ico/zoom-in.png")));
//	actZoomOut->setIcon(QIcon(QString().fromUtf8(":/ico/zoom-out.png")));
//	actZoomExtend->setIcon(QIcon(QString().fromUtf8(":/ico/zoom-extend.png")));
//
//	_toolBar->addAction(actZoomIn);
//	_toolBar->addAction(actZoomOut);
//	_toolBar->addAction(actZoomExtend);
}

void MainWindow::setupStatusBar ()
{
	QStatusBar * statusBar = this->statusBar();
	statusBar->setSizeGripEnabled(true);
}

void MainWindow::setupWidgets ()
{
	QLabel * label = new QLabel("Hello, World!");
	setCentralWidget(label);
}

MainWindow::MainWindow()
	: QMainWindow()
{
	setupMenuBar();
	setupToolBar();
	setupStatusBar();
	setupWidgets();

	setWindowTitle(QString::fromUtf8(_Tr("Hello, World! Application")));
	this->showMaximized();
}
