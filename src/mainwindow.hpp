#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QMainWindow>
#include <QTextEdit>
#include <QTreeWidget>

//class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTextEdit * _editorWidget;
//    QList<ToolBar*> toolBars;
    QMenu * dockWidgetMenu;
    QMenu * mainWindowMenu;
    QSignalMapper * mapper;
    QList<QDockWidget *> extraDockWidgets;
    QAction * createDockWidgetAction;
    QMenu * destroyDockWidgetMenu;

public:
    MainWindow (QWidget *parent = 0, Qt::WindowFlags flags = 0);

protected:
    void showEvent (QShowEvent * event);

public slots:
    void actionTriggered (QAction * action);
    void saveLayout ();
    void loadLayout ();
    void setCorner (int id);
//    void switchLayoutDirection();
    void setDockOptions ();

    void createDockWidget ();
    void destroyDockWidget (QAction *action);

private:
//    void setupToolBar ();
    void setupMenuBar ();
    void setupDockWidgets ();
};

#endif // __MAINWINDOW_HPP__
