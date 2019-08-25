#include "openglmainwindow.h"
#include <QDebug>
#include <QHBoxLayout>
OpenGLMainWindow::OpenGLMainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    win = new OpenGLWindow;
    win->setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);

    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->addWidget(win);

    setCentralWidget(container);
}

OpenGLMainWindow::~OpenGLMainWindow()
{
    delete win;
}
