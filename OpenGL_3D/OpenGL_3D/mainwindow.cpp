#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    win = new OpenGLWidget(this);
    resize(600,600);
    setCentralWidget(win);
}

MainWindow::~MainWindow()
{

}
