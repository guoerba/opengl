#include "openglwindow.h"
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
    "void main(){\n"
         "outputcolor = texture(ourTexture,TexCoord);\n"
    "}\n"//我们使用GLSL内建的texture函数来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
};       //texture函数会使用之前设置的纹理参数对相应的颜色值进行采样。这个片段着色器的输出就是纹理的（插值）纹理坐标上的(过滤后的)颜色。
OpenGLWindow::OpenGLWindow()
    : QOpenGLWindow(0)
    , VBO(0)
    , VAO(0)
    , EBO(0)
    , texture(0)
    , vertexshader(0)
    , fragmentshader(0)
    , shaderprogram(0)
{
   /* QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(4,5);
    format.setProfile(QSurfaceFormat::CoreProfile);

    this->setFormat(format);*/
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0,1.0,0.0,1.0);

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    LoadTextures(QImage("E:/QT/QTproject/OpenGL/OpenGL_learing3/OpenGL_learning3/fatboy.png"));

    GLfloat vertices[] = {
    //---(X,Y)----------(R,G,B)--------(S,T)----
      -0.7f, 0.7f,  1.0f,0.0f,0.0f,  0.0f,1.0f,
       0.7f, 0.7f,  0.0f,1.0f,0.0f,  1.0f,1.0f,
       0.7f,-0.7f,  0.0f,0.0f,1.0f,  1.0f,0.0f,
      -0.7f,-0.7f,  1.0f,0.0f,1.0f,  0.0f,0.0f
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

    QMap<GLenum,GLchar*> map;
    map[GL_VERTEX_SHADER] = vertexShader;
    map[GL_FRAGMENT_SHADER] = fragmentShader;
    ShaderProgram(map);

    glVertexAttribPointer(0,2,GL_FLOAT,FALSE,7*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,FALSE,7*sizeof(GLfloat),(void *)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,FALSE,7*sizeof(GLfloat),(void*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderprogram);

    glBindTexture(GL_TEXTURE_2D,texture);
    errorDebug();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
    glBindVertexArray(0);
}

void OpenGLWindow::ShaderProgram(QMap<GLenum, GLchar *> program)
{
    QList<GLenum> key = program.keys();
    for(int i=0;i<program.size();i++)
    {
        if(key.at(i) == GL_VERTEX_SHADER)
        {
            vertexshader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexshader,1,&program[GL_VERTEX_SHADER],NULL);
            glCompileShader(vertexshader);
            qDebug() << tr("vertex shader %1:").arg(vertexshader) << GetShaderCompileError(vertexshader);
        }
        else if(key.at(i) == GL_FRAGMENT_SHADER)
        {
            fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentshader,1,&program[GL_FRAGMENT_SHADER],NULL);
            glCompileShader(fragmentshader);
            qDebug() << tr("fragment shader %1:").arg(fragmentshader) << GetShaderCompileError(fragmentshader);
        }
    }
    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram,vertexshader);
    glAttachShader(shaderprogram,fragmentshader);
    glLinkProgram(shaderprogram);
    qDebug() << tr("shader program %1:").arg(shaderprogram) << GetProgramError(shaderprogram);

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
        QString message("no problem");
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
        QString message("no problem");
        return message;
    }
}

unsigned char *OpenGLWindow::load_image(const QImage &image, QImage::Format format)
{
    if(image.isNull())
    {
        qWarning("image do not exist");
        return NULL;
    }
    qDebug() << "image was successfully loaded !";
    QImage glimage = image.convertToFormat(format);
    return glimage.bits();
}

void OpenGLWindow::LoadTextures(QImage &image)
{
    glGenTextures(1,&texture);//获取纹理对象
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
    glBindTexture(GL_TEXTURE_2D,0);
    errorDebug();
}

void OpenGLWindow::errorDebug()
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
