#include "MainWindow.hpp"
#include "colorswatch.hpp"
#include "toolbar.hpp"

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
//#include <QTextEdit>
//#include <QFile>
//#include <QDataStream>
//#include <QFileDialog>
//#include <QMessageBox>
//#include <QSignalMapper>
//#include <QApplication>
//#include <QPainter>
//#include <QMouseEvent>
//#include <QLineEdit>
//#include <QComboBox>
//#include <QLabel>
//#include <QPushButton>
#include <QListView>


#include <QtGui>
#include "Application.hpp"
#include "Stencil.hpp"
#include "SchemeView.hpp"
#include "StencilListModel.hpp"

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

MainWindow::MainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setWindowTitle("rw-meccano");

    setupToolBar();
    setupMenuBar();
    setupWidgets();
    statusBar()->showMessage(_Tr("Status Bar"));
}


void MainWindow::setupToolBar()
{
/*
    for (int i = 0; i < 3; ++i) {
        ToolBar *tb = new ToolBar(QString::fromLatin1("Tool Bar %1").arg(i + 1), this);
        toolBars.append(tb);
        addToolBar(tb);
    }
*/
}


void MainWindow::setupMenuBar()
{
    QMenu * menu = menuBar()->addMenu(_Tr("&File"));
//    QAction * action = nullptr;

//    action = menu->addAction(tr("Save layout..."));
//    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

//    action = menu->addAction(tr("Load layout..."));
//    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));

//    action = menu->addAction(tr("Switch layout direction"));
//    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

//    menu->addSeparator();
    menu->addAction(_Tr("&Quit"), this, SLOT(close()));

/*    mainWindowMenu = menuBar()->addMenu(tr("Main window"));

    action = mainWindowMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowNestedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

    action = mainWindowMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));*/

//    QMenu *toolBarMenu = menuBar()->addMenu(tr("Tool bars"));
//    for (int i = 0; i < toolBars.count(); ++i)
//        toolBarMenu->addMenu(toolBars.at(i)->menu);

//    dockWidgetMenu = menuBar()->addMenu(tr("&Dock Widgets"));
}

#ifdef __COMMENT__
void MainWindow::saveLayout()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::loadLayout()
{
    QString fileName
        = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
        ok = restoreGeometry(geo_data);
    if (ok)
        ok = restoreState(layout_data);

    if (!ok) {
        QString msg = tr("Error reading %1")
                        .arg(fileName);
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

#endif


/*
class StencilListView : public QListView
{
public:
	StencilListVew (QWidget * parent = 0) : QListView(parent) {}

protected:
	virtual void startDrag (Qt::DropActions supportedActions) override
	{
		QListView::startDrag(supportedActions);
	}

    virtual void dragEnterEvent (QDragEnterEvent * event) override
    {
    	QListView::dragEnterEvent(event);
    }

    virtual void dragLeaveEvent (QDragLeaveEvent * event) override
	{
    	QListView::dragLeaveEvent(event);
	}

    virtual void dragMoveEvent (QDragMoveEvent * event) override
	{
        if (event->source() != this) {
        	event->accept();
        } else {
        	event->ignore();
        }
	}

    virtual void dropEvent (QDropEvent * event) override
    {
    	QListView::dropEvent(event);
    }

    virtual void mousePressEvent (QMouseEvent * event) override
	{
    	QListView::mousePressEvent(event);
	}
};
*/


void MainWindow::setupWidgets ()
{
    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    _editorWidget = new SchemeView(this);
    _editorWidget->setMinimumSize(400, 205);
    setCentralWidget(_editorWidget);

    _stencilListView = new QListView;
    _stencilListView->setDragEnabled(true);
    _stencilListView->setViewMode(QListView::IconMode);
    _stencilListView->setIconSize(QSize(Stencil::Size, Stencil::Size));
    _stencilListView->setGridSize(QSize(Stencil::Size + 10, Stencil::Size + 10));
//    _stencilListView->setSpacing(10);
//    _stencilListView->setMovement(QListView::Free);
    _stencilListView->setAcceptDrops(true);
    _stencilListView->setDropIndicatorShown(true);

    _stencilListModel = new StencilListModel(this);
    _stencilListModel->populateStencils();
    _stencilListView->setModel(_stencilListModel);

    QDockWidget * stencilDockWidget = new QDockWidget(_Tr("Stencil List"), this, 0);
    addDockWidget(Qt::LeftDockWidgetArea, stencilDockWidget);
    stencilDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    stencilDockWidget->setWidget(_stencilListView);
    _stencilListView->show();

}
