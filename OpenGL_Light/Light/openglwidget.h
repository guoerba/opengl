﻿#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QColor>
#include <QTimer>
#include "vertices.h"
#include "shader.h"
#include "camera.h"
#include "sphere.h"

#define lightcolor (QVector3D(1.0f,1.0f,1.0f))

class OpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w,int h);
private:
    GLuint VAO,VBO,EBO;
    GLuint VAO_Sphere,VBO_Sphere,EBO_Sphere;
    GLuint VAO_Untexture;
    int vnumber;

    void ShaderProgram(const char *vshader, const char *fshader,QOpenGLShaderProgram *pro);
    QOpenGLShaderProgram *program,*programforSphere,*programforUntexture;

    Camera *viewcamera;//摄像机
    void SetModelMatrix(int index,QOpenGLShaderProgram *pro);//模型矩阵（物体空间——世界空间）
    void SetViewMatrix(QOpenGLShaderProgram *pro);//观察矩阵（世界空间——观察空间）
    void SetProjectionMatrix(int width,int height,QOpenGLShaderProgram *pro);//投影矩阵（观察空间——切割空间）（将三维坐标映射到2维坐标）

    QTimer *timer;//刷新
    void SetSphereVertex();
    void SetUntextureVertex();

    QOpenGLTexture *texture[2];//纹理
    void SetTexture();

    int count;//全局变量
};

#endif // OPENGLWIDGET_H

