#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTextCodec>
#include "openglwindow.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainwindow(QWidget *parent = 0);
    ~mainwindow();

    void CreateMenuBars();
signals:

public slots:
private:
    OpenGLWindow *openglwin;

    QAction *openAction;
    QAction *saveAction;

    QMenu *startMenu;
};

#endif // MAINWINDOW_H
