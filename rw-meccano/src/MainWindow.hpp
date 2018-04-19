#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QMainWindow>
//#include <QTextEdit>
//#include <QTreeWidget>

//class ToolBar;
class QAction;
class QMenu;
//class QSignalMapper;
class QListView;
class SchemeView;
class QShowEvent;
class StencilListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    SchemeView * _editorWidget;
    QList<QDockWidget *> _extraDockWidgets;

    QListView        * _stencilListView;
    StencilListModel * _stencilListModel;

//    QList<ToolBar*> toolBars;
//    QMenu * dockWidgetMenu;
//    QMenu * mainWindowMenu;
//    QSignalMapper * mapper;
//    QAction * createDockWidgetAction;
//    QMenu * destroyDockWidgetMenu;

public:
    MainWindow (QWidget *parent = 0, Qt::WindowFlags flags = 0);


protected:
//    void showEvent (QShowEvent * event);


public slots:
//    void actionTriggered (QAction * action);
//    void saveLayout ();
//    void loadLayout ();
//    void setCorner (int id);
//    void switchLayoutDirection();
//    void setDockOptions ();

//    void createDockWidget ();
//    void destroyDockWidget (QAction *action);

private:
    void setupToolBar ();
    void setupMenuBar ();
    void setupWidgets ();
};

#endif // __MAINWINDOW_HPP__
