#ifndef OPENGLMAINWINDOW_H
#define OPENGLMAINWINDOW_H

#include <QMainWindow>
#include "openglwindow.h"

class OpenGLMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    OpenGLMainWindow(QWidget *parent = 0);
    ~OpenGLMainWindow();
private:
    OpenGLWindow *win;
};

#endif // OPENGLMAINWINDOW_H
