#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    win = new OpenGLWidget(this);
    setCentralWidget(win);
    resize(600,600);
}

MainWindow::~MainWindow()
{

}
