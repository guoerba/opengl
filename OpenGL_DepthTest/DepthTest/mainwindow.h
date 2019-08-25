#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QAction>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QDialog>
#include "openglwidget.h"
#include "insertdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void InsertNewItem();
    void DeleteItem();
    void ReceiveData(QVector3D location, QVector4D rotate, QVector3D scale, QString dir, QString path);
private:
    OpenGLWidget *glwin;

    QAction *insertnewitemAction;//插入新物品
    QAction *deleteexistitemAction;//删除已有物品

    QSplitter *mainwin;

    QTableWidget *tableforItem;

    QPushButton *insertnewitemButton;
    QPushButton *deleteexistitemButton;

    void CreateAction();
    void CreateMenuBar();
    void CreateConnection();

    QGroupBox* CreateGroupBoxforList();
    QGroupBox* CreateGroupBoxforButton();
    QSplitter* CreateSplitter(QGroupBox* win1, QGroupBox* win2, Qt::Orientation direction = Qt::Horizontal);

};

#endif // MAINWINDOW_H
