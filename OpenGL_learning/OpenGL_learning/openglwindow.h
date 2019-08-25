#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMap>
#include <QSurfaceFormat>

//static GLchar vertexShaderSource[];
//static GLchar FragmentShaderSource[];

#define VAOsNumber 1//VAO数量
#define VBOsNumber 1//VBO数量
#define VerticesNumber 6//顶点数量
#define TriangleVerticesIndex 0//三角形顶点引索，和顶点着色器中的location=0对应

class OpenGLWindow : public QOpenGLWidget,protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWidget *parent = 0);
    ~OpenGLWindow();

    enum Shape_ID{
        Triangle,
        Rectangle
    };

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
private:
    void ShaderProgram(QMap<GLenum,GLchar*> program);
    QString GetShaderCompileError(GLuint shader);
    QString GetProgramError(GLuint program);
    GLuint VAOs[VAOsNumber];//顶点数组对象(VAO)，任何随后的顶点属性调用都会储存在这个VAO中。
    //一个顶点数组对象会储存以下这些内容：
    //glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
    //通过glVertexAttribPointer设置的顶点属性配置。
    //通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
    GLuint VBO;//顶点缓冲对象(VBO)
    GLuint vertexshader;
    GLuint fragmentshader;
    GLuint shaderprogram;
};

#endif // OPENGLWINDOW_H
