#ifndef INSERTDIALOG_H
#define INSERTDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QVector3D>
#include <QVector4D>

class InsertDialog : public QDialog
{
    Q_OBJECT
public:
    enum InsertFormat{//枚举类型做函数参数时要在函数之前声明，而且枚举类型参数不能有重复
        INSERT_Dialog,
        DELETE_Dialog
    };
    explicit InsertDialog(InsertFormat format = INSERT_Dialog, QWidget *parent = 0);
signals:
    void transmitData(QVector3D location, QVector4D rotate, QVector3D scale, QString dir, QString path);
public slots:
    void CreateFileDialog();
private:
    QGroupBox *CreateInsertGroupBtn();
    QGroupBox *CreateModelPathGroup();
    QGroupBox *CreateTranslateGroup();
    QGroupBox *CreateRotateGroup();
    QGroupBox *CreateScaleGroup();
    void CreateConnection(QWidget *parent = 0);


    QString dir,path;

    QLineEdit *modelPath;
    QLineEdit *Line_Translate_x;
    QLineEdit *Line_Translate_y;
    QLineEdit *Line_Translate_z;
    QLineEdit *Line_Rotate_x;
    QLineEdit *Line_Rotate_y;
    QLineEdit *Line_Rotate_z;
    QLineEdit *Line_Rotate_angle;
    QLineEdit *Line_Scale_x;
    QLineEdit *Line_Scale_y;
    QLineEdit *Line_Scale_z;

    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *modelPathBtn;
};

#endif // INSERTDIALOG_H
