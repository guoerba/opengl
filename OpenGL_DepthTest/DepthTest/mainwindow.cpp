#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{        
    glwin = new OpenGLWidget(this);

    mainwin = new QSplitter(Qt::Horizontal,this);
    mainwin->addWidget(glwin);
    mainwin->addWidget(CreateSplitter(CreateGroupBoxforList(),
                                      CreateGroupBoxforButton(),
                                      Qt::Vertical));
    mainwin->setStretchFactor(0,3);
    mainwin->setStretchFactor(1,1);

    setCentralWidget(mainwin);
    resize(600,600);

    CreateMenuBar();
    CreateConnection();
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateAction()
{
    insertnewitemAction = new QAction(tr("新增物体"),this);
    deleteexistitemAction = new QAction (tr("删除物体"),this);
}

void MainWindow::CreateMenuBar()
{
    CreateAction();

    QMenu *startMenu = this->menuBar()->addMenu(tr("开始菜单"));
    QList<QAction*> actionlist;
    actionlist << insertnewitemAction << deleteexistitemAction;
    startMenu->addActions(actionlist);
}

void MainWindow::CreateConnection()
{
    connect(insertnewitemButton,SIGNAL(clicked(bool)),this,SLOT(InsertNewItem()));
    connect(insertnewitemAction,SIGNAL(triggered(bool)),this,SLOT(InsertNewItem()));
    connect(deleteexistitemButton,SIGNAL(clicked(bool)),this,SLOT(DeleteItem()));
    connect(deleteexistitemAction,SIGNAL(triggered(bool)),this,SLOT(DeleteItem()));
}

void MainWindow::InsertNewItem()
{
    InsertDialog dialog(InsertDialog::INSERT_Dialog,this);
    connect(&dialog,SIGNAL(transmitData(QVector3D,QVector4D,QVector3D,QString,QString)),
            this,SLOT(ReceiveData(QVector3D,QVector4D,QVector3D,QString,QString)));
    dialog.exec();
}

void MainWindow::DeleteItem()
{

}

QGroupBox* MainWindow::CreateGroupBoxforList()
{
    QGroupBox *group = new QGroupBox(tr("物体列表"),this);

    QStringList header = QStringList() << tr("name") << tr("location") << tr("source file path");

    tableforItem = new QTableWidget(0,3);
    tableforItem->setHorizontalHeaderLabels(header);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableforItem);

    group->setLayout(layout);

    return group;
}

QGroupBox* MainWindow::CreateGroupBoxforButton()
{
    QGroupBox *group = new QGroupBox(tr("工具栏"),this);

    insertnewitemButton = new QPushButton(tr("插入模型"));
    deleteexistitemButton = new QPushButton(tr("删除模型"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(insertnewitemButton,0,0);
    layout->addWidget(deleteexistitemButton,0,1);
    group->setLayout(layout);

    return group;
}

QSplitter* MainWindow::CreateSplitter(QGroupBox *win1, QGroupBox *win2,Qt::Orientation direction)
{
    QSplitter *splitter = new QSplitter(direction,this);
    splitter->addWidget(win1);
    splitter->addWidget(win2);
    return splitter;
}
//将新模型信息导入opengl窗口
void MainWindow::ReceiveData(QVector3D location, QVector4D rotate, QVector3D scale, QString dir, QString path)
{
    qDebug() << "location: " << location;
    qDebug() << "directory: " << dir;
    qDebug() << "path: " << path;
    glwin->ReceiveData(location,rotate,scale,dir,path);
}
