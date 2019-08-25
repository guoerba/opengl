#include "openglwindow.h"
#include <QDebug>
static GLchar vertexShader[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec2 position;\n"
    "layout(location = 1) in vec4 color;\n "
    "out vec4 glcolor; \n"
    "void main() {\n"
    "gl_Position = vec4(position,0.0f,1.0f); \n"
    "glcolor = color; \n"
    "}\n"
};
static GLchar fragmentShader[] = {
    "#version 450 core\n"
    "in vec4 glcolor; \n"
    "out vec4 fragmentcolor;\n"
    "uniform vec4 addcolor;\n"
    "void main() {\n"
    "fragmentcolor = glcolor + addcolor;\n "
    "}\n"
};

OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,5);

    setFormat(format);
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5,0.5,0.5,1.0);

    glGenVertexArrays((GLsizei)VAOsNumber,VAOs);
    glBindVertexArray(VAOs[0]);

    GLfloat RectangleVertiecs[4][6] = {
        {-0.5f, 0.5f,1.0f,0.0f,0.0f,1.0f},
        { 0.5f, 0.5f,0.0f,1.0f,0.0f,1.0f},
        { 0.5f,-0.5f,0.0f,0.0f,1.0f,1.0f},
        {-0.5f,-0.5f,1.0f,1.0f,0.0f,1.0f}
    };

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(RectangleVertiecs),RectangleVertiecs,GL_STATIC_DRAW);

    GLuint indices[2][3] = {//EBO决定以什么顺序输出顶点
        {0,1,2},
        {2,3,0}
    };

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    QMap<GLsizei,GLchar*> shadermap;
    shadermap[GL_VERTEX_SHADER] = vertexShader;
    shadermap[GL_FRAGMENT_SHADER] = fragmentShader;
    programShader(shadermap);

    glVertexAttribPointer(0,2,GL_FLOAT,FALSE,6*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,FALSE,6*sizeof(GLfloat),(void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
 //   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBindVertexArray(0);

    time = new QTime;
    time->start();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(100);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderprogram);

    changeUniformVariant(shaderprogram);

    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
 //   glDrawArrays(GL_TRIANGLE_FAN ,0,4);//GL_TRIANGLE_FAN参数可以形成矩形
    glBindVertexArray(0);
}

void OpenGLWindow::programShader(QMap<GLsizei, GLchar *> program)
{
    QList<GLsizei> key = program.keys();
    for(int i=0;i<key.size();i++)
    {
        if(key.at(i) == GL_VERTEX_SHADER)
        {
            vertexshader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexshader,1,&program[GL_VERTEX_SHADER],NULL);
            glCompileShader(vertexshader);
            qDebug() << tr("顶点着色器%1:").arg(vertexshader) << GetShaderCompileError(vertexshader);
        }
        else if(key.at(i) == GL_FRAGMENT_SHADER)
        {
            fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentshader,1,&program[GL_FRAGMENT_SHADER],NULL);
            glCompileShader(fragmentshader);
            qDebug() << tr("片段着色器%1:").arg(fragmentshader) << GetShaderCompileError(fragmentshader);
        }
    }
    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram,vertexshader);
    glAttachShader(shaderprogram,fragmentshader);
    glLinkProgram(shaderprogram);
    qDebug() << tr("着色器程序%1:").arg(shaderprogram) << GetProgramError(shaderprogram);

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);


}
QString OpenGLWindow::GetShaderCompileError(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        GLchar log[512];
        glGetShaderInfoLog(shader,512,NULL,log);
        QString message(log);
        return message;
    }
    else
    {
        QString message("没有问题");
        return message;
    }
}

QString OpenGLWindow::GetProgramError(GLuint program)
{
    GLint success;
    glGetProgramiv(program,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        GLchar log[512];
        glGetProgramInfoLog(program,512,NULL,log);
        QString message(log);
        return message;
    }
    else
    {
        QString message("没有问题");
        return message;
    }
}

void OpenGLWindow::changeUniformVariant(GLuint program)
{
    GLfloat red = sin(time->elapsed()%10)/5;
    GLfloat green = sin(time->elapsed()%10)/5;
    GLfloat blue = sin(time->elapsed()%10)/5;
    GLuint dynamicColorLocation = glGetUniformLocation(program,"addcolor");//获取程序中指定uniform变量的位置
    glUniform4f(dynamicColorLocation,red,green,blue,0.0f);//设置uniform变量的值
}
