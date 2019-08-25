#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QSurfaceFormat>
#include <QImage>

class OpenGLWindow : public QOpenGLWindow,protected QOpenGLFunctions_4_5_Core
{
public:
    explicit OpenGLWindow(/*QWidget *parent = 0*/);
    ~OpenGLWindow();
    unsigned char *load_image(const QImage &image,QImage::Format format);
    void errorDebug();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    GLuint texture;
    GLuint vertexshader;
    GLuint fragmentshader;
    GLuint shaderprogram ;
    void ShaderProgram(QMap<GLenum,GLchar*> program);
    QString GetShaderCompileError(GLuint shader);
    QString GetProgramError(GLuint program);
    void LoadTextures(QImage &image);
};

#endif // OPENGLWINDOW_H
