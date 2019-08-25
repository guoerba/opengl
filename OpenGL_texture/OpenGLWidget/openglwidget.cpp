#include "openglwidget.h"
#include <QDebug>
static GLchar vertexShader[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec2 position;\n"//位置属性
    "layout(location = 1) in vec3 color;\n"   //颜色属性
    "layout(location = 2) in vec2 texcoord;\n"//纹理属性
    "out vec4 colortofragmentshader;\n"
    "out vec2 TexCoord; \n"
    "void main(){\n"
        "gl_Position = vec4(position,0.0,1.0);\n"
        "colortofragmentshader = vec4(color,1.0);\n"
        "TexCoord = texcoord; \n"
    "}\n"
};

static GLchar fragmentShader[] = {//片段着色器也应该能访问纹理对象，但是我们怎样能把纹理对象传给片段着色器呢？
    "#version 450 core\n"         //GLSL有一个供纹理对象使用的内建数据类型，叫做采样器(Sampler)，它以纹理类型作为后缀，
    "in vec4 colortofragmentshader;\n"//比如sampler1D、sampler3D，或在我们的例子中的sampler2D。
    "in vec2 TexCoord; \n"         //我们可以简单声明一个uniform sampler2D把一个纹理添加到片段着色器中，稍后我们会把纹理赋值给这个uniform。
    "out vec4 outputcolor;\n"
    "uniform sampler2D ourTexture; \n"
    "uniform vec4 addcolor;"
    "void main(){\n"
         "outputcolor = texture(ourTexture,TexCoord)*colortofragmentshader;\n"
    "}\n"//我们使用GLSL内建的texture函数来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
};       //texture函数会使用之前设置的纹理参数对相应的颜色值进行采样。这个片段着色器的输出就是纹理的（插值）纹理坐标上的(过滤后的)颜色。

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,5);

    setFormat(format);
}

OpenGLWidget::~OpenGLWidget()
{
    texture->destroy();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5,0.5,0.5,1.0);

    glGenVertexArrays((GLsizei)VAOsNumber,VAOs);
    glBindVertexArray(VAOs[0]);

  //  texture_img.load("E:/QT/QTproject/OpenGL/OpenGL_texture/OpenGLWidget/box.jpg");
    LoadTextures(QImage(tr("E:/QT/QTproject/OpenGL/OpenGL_texture/OpenGLWidget/box.jpg")));

    GLfloat RectangleVertiecs[4][8] = {
        {-0.5f, 0.5f,   1.0f,0.0f,0.0f,1.0f,  0.0f,1.0f},
        { 0.5f, 0.5f,   0.0f,1.0f,0.0f,1.0f,  1.0f,1.0f},
        { 0.5f,-0.5f,   0.0f,0.0f,1.0f,1.0f,  1.0f,0.0f},
        {-0.5f,-0.5f,   1.0f,1.0f,0.0f,1.0f,  0.0f,0.0f}
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

    glVertexAttribPointer(0,2,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
 //   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);


    time = new QTime;
    time->start();

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(100);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderprogram);

    texture->bind();
    glBindVertexArray(VAOs[0]);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
    glBindVertexArray(0);
}

void OpenGLWidget::programShader(QMap<GLsizei, GLchar *> program)
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
QString OpenGLWidget::GetShaderCompileError(GLuint shader)
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

QString OpenGLWidget::GetProgramError(GLuint program)
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

void OpenGLWidget::changeUniformVariant(GLuint program)
{
    GLfloat red = sin(time->elapsed()%10)/2;
    GLfloat green = sin(time->elapsed()%10)/2;
    GLfloat blue = sin(time->elapsed()%10)/2;
    if(fabsf(red)<0.3)
    {
        red = green = blue = 0.3;
    }
    qDebug() << red;
    GLuint dynamicColorLocation = glGetUniformLocation(program,"addcolor");//获取程序中指定uniform变量的位置
    glUniform4f(dynamicColorLocation,red,green,blue,1.0f);//设置uniform变量的值
}

/*unsigned char *OpenGLWidget::load_image(const QImage &image, QImage::Format format)
{
    if(image.isNull())
    {
        qWarning("image do not exist");
        return NULL;
    }
    qDebug() << "image was successfully loaded !";
    QImage glimage = image.convertToFormat(format);
    return glimage.bits();
}*/

void OpenGLWidget::LoadTextures(QImage &image)
{
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    texture->setData(image);
    qDebug() << "samples:" << texture->samples();
  /*  glGenTextures(1,&texture);//获取纹理对象
    glBindTexture(GL_TEXTURE_2D,texture);//绑定纹理对象
    //为之前绑定的纹理对象的S(X),T(Y),R(Z)轴设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    //为之前绑定的纹理对象的放大(Magnify)和缩小(Minify)设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//多级渐远纹理之间的过滤，只可用于缩小过滤
    //加载纹理
    unsigned char *img = load_image(image,QImage::Format_RGB888);
    //当前绑定的纹理对象就会被附加上纹理
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image.width(),image.height(),0,GL_RGB,GL_UNSIGNED_BYTE,img);
    //为当前绑定的纹理自动生成所有需要的多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    errorDebug();*/
}

void OpenGLWidget::errorDebug()
{
    GLenum error = glGetError();
    switch(error)
    {
        case 0:qDebug() << tr("error %1:").arg(error) << tr("GL_NO_ERROR");break;
        case 1:qDebug() << tr("error %1:").arg(error) << tr("GL_INVALID_ENUM");break;
        case 2:qDebug() << tr("error %1:").arg(error) << tr("GL_INVALID_VALUE");break;
        case 3:qDebug() << tr("error %1:").arg(error) << tr("GL_INVALID_OPERATION");break;
        case 4:qDebug() << tr("error %1:").arg(error) << tr("GL_INVALID_FRAMEBUFFER_OPERATION");break;
        case 5:qDebug() << tr("error %1:").arg(error) << tr("GL_OUT_OF_MEMORY");break;
        case 6:qDebug() << tr("error %1:").arg(error) << tr("GL_STACK_UNDERFLOW");break;
        case 7:qDebug() << tr("error %1:").arg(error) << tr("GL_STACK_OVERFLOW");break;
    }
}
