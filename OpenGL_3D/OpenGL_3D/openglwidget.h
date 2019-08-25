#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QDebug>
#include <QImage>
#include <QTime>
#include <QTimer>
#include "verticesarray.h"
#include "camera.h"

#define camerardius 50

class OpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
public:
    explicit OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
  //  bool event(QEvent *e);
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    QOpenGLTexture *texture[2];
    QOpenGLShaderProgram *program;
    Camera *viewcamera;

    void ShaderProgram();
    void SetTexture();
    void SetModelMatrix(int index);
    void SetViewMatrix();
    void SetProjectionMatrix(int width,int height);
    QTime *time;
    int count = 0;

};

#endif // OPENGLWIDGET_H
