#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
     OpenGLWidget *win;

     QAction *openaction;
     QAction *saveaction;

     QMenu *startmenu;

     void CreateMenuBar();
};

#endif // MAINWINDOW_H
