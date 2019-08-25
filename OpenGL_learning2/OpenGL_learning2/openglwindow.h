#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#define VAOsNumber 1

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QMap>
#include <QSurfaceFormat>
#include <QTimer>
#include <QTime>

class OpenGLWindow : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWidget *parent = 0);
    ~OpenGLWindow();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
private:
    void programShader(QMap<GLsizei,GLchar*> program);
    QString GetShaderCompileError(GLuint shader);
    QString GetProgramError(GLuint program);
    void changeUniformVariant(GLuint program);

    GLuint VBO;
    GLuint VAOs[VAOsNumber];
    GLuint EBO;//引索缓冲对象，可以用来计算重复的顶点
    GLuint vertexshader,fragmentshader,shaderprogram;
    QTime *time;
};

#endif // OPENGLWINDOW_H
