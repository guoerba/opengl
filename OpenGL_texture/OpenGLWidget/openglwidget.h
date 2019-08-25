#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#define VAOsNumber 1

#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QDebug>
#include <QTime>
#include <QTimer>

class OpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
    void errorDebug();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
private:
    void programShader(QMap<GLsizei,GLchar*> program);
    QString GetShaderCompileError(GLuint shader);
    QString GetProgramError(GLuint program);
    void changeUniformVariant(GLuint program);
  /*  unsigned char *load_image(const QImage &image,QImage::Format format);*/
    void LoadTextures(QImage &image);

    GLuint VBO;
    GLuint VAOs[VAOsNumber];
    GLuint EBO;//引索缓冲对象，可以用来计算重复的顶点
    GLuint vertexshader,fragmentshader,shaderprogram;
    QOpenGLTexture *texture;
    QTime *time;
  //  QImage texture_img;

};

#endif // OPENGLWIDGET_H
