#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent)
{

  /*  CreateMenuBars();

    openglwin = new OpenGLWindow;
    setCentralWidget(openglwin);*/
}

mainwindow::~mainwindow()
{
    delete openglwin;
}

void mainwindow::CreateMenuBars()
{

    openAction = new QAction(tr("open"),this);
    saveAction = new QAction(tr("save"),this);

    startMenu = new QMenu(this);
    QString name = QString::fromUtf8("start");

    startMenu = menuBar()->addMenu(name);
    QList<QAction*> actions;
    actions << openAction << saveAction;
    startMenu->addActions(actions);
}
