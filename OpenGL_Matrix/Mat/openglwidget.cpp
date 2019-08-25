#include "openglwidget.h"
static const char vertexshadersource[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec3 vertex;\n"
    "layout(location = 1) in vec2 textcoord;\n "
    "out vec2 TexCoord;\n "
    "uniform mat4 transform;\n "//变换矩阵;
    "void main(){\n"
    "gl_Position = transform*vec4(vertex,1.0f); \n"
    "TexCoord = textcoord;\n "
    "}\n "
};

static const char fragmentshadersource[] = {
  "#version 450 core\n"
    "in vec2 TexCoord;\n "
    "uniform sampler2D Tex1;\n"
    "uniform sampler2D Tex2;\n "
    "out vec4 color;\n"
    "void main(){\n"
    "color = mix(texture(Tex1,TexCoord),texture(Tex2,TexCoord),0.2);\n "
    "}\n "
};

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(4,5);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);
}

OpenGLWidget::~OpenGLWidget()
{
    program->destroyed();
    texture[0]->destroy();
    texture[1]->destroy();

}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.4f,0.2f,0.6f,1.0f);

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    GLfloat vertices[] = {
      -0.3f, 0.3f, 0.0f,    0.0f,1.0f,
       0.3f, 0.3f, 0.0f,    1.0f,1.0f,
       0.3f,-0.3f, 0.0f,    1.0f,0.0f,
      -0.3f,-0.3f, 0.0f,    0.0f,0.0f
    };
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    GLuint indices[] = {
      0,1,2,
      0,2,3
    };
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    ShaderProgram();

    SetTexture();//设置纹理
    glVertexAttribPointer(0,3,GL_FLOAT,FALSE,5*sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,FALSE,5*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    time = new QTime;
    time->start();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1);

}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    SetTransform();

    texture[0]->bind(0);
    texture[1]->bind(1);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
    glBindVertexArray(0);
    texture[0]->release(0);
    texture[1]->release(1);
}

void OpenGLWidget::ShaderProgram()
{
    program = new QOpenGLShaderProgram(this);
    if(program->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexshadersource))
        qDebug() << tr("no error");
    else
        qDebug() << tr("vertex shader error：") << program->log();

    if(program->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentshadersource))
        qDebug() << tr("no error");
    else
        qDebug() << tr("fragment shader error：") << program->log();

    if(program->link())//glLinkProgram;glGetProgramiv;glGetProgramInfoLog;
        qDebug() << tr("no error");
    else
        qDebug() << tr("link error:") << program->log();
      program->bind();
}

void OpenGLWidget::SetTexture()
{
    texture[0] = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture[0]->create();//glGenTextures(1, &texture);
    texture[0]->bind(0);//glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D, texture1);
    program->setUniformValue("Tex1",0);//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
    texture[0]->setMagnificationFilter(QOpenGLTexture::Linear);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture[0]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture[0]->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    texture[0]->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    texture[0]->setData(QImage("E:/QT/QTproject/OpenGL/OpenGL_Matrix/Mat/box.jpg"));//glTexSubImage2D
    texture[0]->release(0);

    texture[1] = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture[1]->create();
    texture[1]->bind(1);
    program->setUniformValue("Tex2",1);
    texture[1]->setMagnificationFilter(QOpenGLTexture::Linear);
    texture[1]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture[1]->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture[1]->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    texture[1]->setFormat(QOpenGLTexture::SRGB_DXT1);
    texture[1]->setData(QImage("E:/QT/QTproject/OpenGL/OpenGL_Matrix/Mat/awesomeface.png").mirrored());
    texture[1]->release(1);
}

void OpenGLWidget::SetTransform()
{
    QMatrix4x4 mat;
//当矩阵相乘时，在最右边的矩阵是第一个与向量相乘的，所以你应该从右向左读这个乘法。
//建议您在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，否则它们会（消极地）互相影响。
    mat.translate(0.6*sin(((float)count)/57.2),0.6*cos(((float)count)/57.2));
    mat.rotate((float)count,0,0,1);
    mat.scale(1.2*sin(2*((float)count)/57.2));
    if(count >= 360)
        count = 0;
    count++;
 //   qDebug() << mat;
    program->setUniformValue(program->uniformLocation("transform"),mat);

}
