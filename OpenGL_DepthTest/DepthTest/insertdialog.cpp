#include "insertdialog.h"

InsertDialog::InsertDialog(InsertFormat format, QWidget *parent)
    : QDialog(parent)
{    
    if(format == INSERT_Dialog)//插入模型对话框
    {
        QGridLayout *layout = new QGridLayout(this);
        layout->addWidget(CreateModelPathGroup(),0,0,1,3);
        layout->addWidget(CreateTranslateGroup(),1,0);
        layout->addWidget(CreateRotateGroup(),1,1);
        layout->addWidget(CreateScaleGroup(),1,2);
        layout->addWidget(CreateInsertGroupBtn(),2,0,1,3);

        CreateConnection();
        //layout->setSizeConstraint(QLayout::SetFixedSize);

    }
    else if(format == DELETE_Dialog)//删除模型对话框
    {

    }

}

QGroupBox* InsertDialog::CreateInsertGroupBtn()
{
    QGroupBox *group = new QGroupBox(tr("Y/N"),this);

    ok = new QPushButton(tr("ok"));
    cancel = new QPushButton(tr("cancel"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(ok);
    layout->addWidget(cancel);
    group->setLayout(layout);

    return group;
}

QGroupBox *InsertDialog::CreateModelPathGroup()
{
    QGroupBox *group = new QGroupBox(tr("path"));

    QLabel *label = new QLabel(tr("模型路径"));
    modelPath = new QLineEdit;
    modelPath->setDisabled(true);
    modelPathBtn = new QPushButton(tr("..."));

    QHBoxLayout *layout = new QHBoxLayout(group);
    layout->addWidget(label);
    layout->addWidget(modelPath);
    layout->addWidget(modelPathBtn);

    return group;
}

QGroupBox *InsertDialog::CreateTranslateGroup()
{
    QGroupBox *group = new QGroupBox(tr("world space location"));

    Line_Translate_x = new QLineEdit;
    Line_Translate_y = new QLineEdit;
    Line_Translate_z = new QLineEdit;

    QFormLayout *layout = new QFormLayout(group);
    layout->addRow(tr("x:"),Line_Translate_x);
    layout->addRow(tr("y:"),Line_Translate_y);
    layout->addRow(tr("z:"),Line_Translate_z);

    return group;
}

QGroupBox *InsertDialog::CreateRotateGroup()
{
    QGroupBox *group = new QGroupBox(tr("wold space rotate"));

    Line_Rotate_x = new QLineEdit;
    Line_Rotate_y = new QLineEdit;
    Line_Rotate_z = new QLineEdit;
    Line_Rotate_angle = new QLineEdit;

    QFormLayout *layout = new QFormLayout(group);
    layout->addRow(tr("axis_x:"),Line_Rotate_x);
    layout->addRow(tr("axis_y:"),Line_Rotate_y);
    layout->addRow(tr("axis_z:"),Line_Rotate_z);
    layout->addRow(tr("angle:"),Line_Rotate_angle);

    return group;
 }

QGroupBox *InsertDialog::CreateScaleGroup()
{
    QGroupBox *group = new QGroupBox(tr("world space scale"));

    Line_Scale_x = new QLineEdit;
    Line_Scale_y = new QLineEdit;
    Line_Scale_z = new QLineEdit;

    QFormLayout *layout = new QFormLayout(group);
    layout->addRow(tr("scale_x:"),Line_Scale_x);
    layout->addRow(tr("scale_y:"),Line_Scale_y);
    layout->addRow(tr("scale_z:"),Line_Scale_z);

    return group;
}

void InsertDialog::CreateConnection(QWidget *parent)
{

     connect(modelPathBtn,SIGNAL(clicked(bool)),this,SLOT(CreateFileDialog()));
     connect(ok,&QPushButton::clicked,
             [=]{
                if(this->close())
                {
                    if(Line_Translate_x->text().isEmpty())
                        Line_Translate_x->setText(QString::number(0.0f));
                    if(Line_Translate_y->text().isEmpty())
                        Line_Translate_y->setText(QString::number(0.0f));
                    if(Line_Translate_z->text().isEmpty())
                        Line_Translate_z->setText(QString::number(0.0f));
                    QVector3D vec3_Translate(Line_Translate_x->text().toFloat(),
                                   Line_Translate_y->text().toFloat(),
                                   Line_Translate_z->text().toFloat());

                    if(Line_Rotate_x->text().isEmpty())
                        Line_Rotate_x->setText(QString::number(0.0f));
                    if(Line_Rotate_y->text().isEmpty())
                        Line_Rotate_y->setText(QString::number(0.0f));
                    if(Line_Rotate_z->text().isEmpty())
                        Line_Rotate_z->setText(QString::number(0.0f));
                    if(Line_Rotate_angle->text().isEmpty())
                        Line_Rotate_angle->setText(QString::number(0.0f));
                    QVector4D vec4_Rotate(Line_Rotate_x->text().toFloat(),
                                          Line_Rotate_y->text().toFloat(),
                                          Line_Rotate_z->text().toFloat(),
                                          Line_Rotate_angle->text().toFloat());

                    if(Line_Scale_x->text().isEmpty())
                        Line_Scale_x->setText(QString::number(1.0f));
                    if(Line_Scale_y->text().isEmpty())
                        Line_Scale_y->setText(QString::number(1.0f));
                    if(Line_Scale_z->text().isEmpty())
                        Line_Scale_z->setText(QString::number(1.0f));
                    QVector3D vec3_Scale(Line_Scale_x->text().toFloat(),
                                         Line_Scale_y->text().toFloat(),
                                         Line_Scale_z->text().toFloat());

                    emit transmitData(vec3_Translate,vec4_Rotate,vec3_Scale,dir,path);
                }
         });
     connect(cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
}

void InsertDialog::CreateFileDialog()
{
    QFileDialog filedialog(this);
    filedialog.setFileMode(QFileDialog::AnyFile);
    filedialog.setNameFilter(tr("model (*.3d *.3ds *.ac *.ASE *.b3d *.blend *.bvh *.cob *.dae *.csm"
                                       "*.dxf *.hmp *.ifc *.irr *.irrmesh *.lwo *.lws *.md2 *.md5mesh"
                                       "*.MDL *.mdl *.ms3d *.nff *.obj *.off *.mesh *.ply *.q3o *smd"
                                       "*.stl *.ter *.wrl *.x *.zgl )"));
    filedialog.setViewMode(QFileDialog::Detail);
    if(filedialog.exec())
    {
        dir = filedialog.directory().absolutePath();
        path = filedialog.selectedFiles().at(0);
        modelPath->setText(path);
    }
}
