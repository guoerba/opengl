#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    win = new OpenGLWidget(this);
    setCentralWidget(win);

    CreateMenuBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateMenuBar()
{
    openaction = new QAction("open",this);
    saveaction = new QAction("save",this);

    startmenu = new QMenu(this);
    startmenu = this->menuBar()->addMenu("start");
    QList<QAction*> actions;
    actions << openaction << saveaction;
    startmenu->addActions(actions);


}
