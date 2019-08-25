#include "openglwindow.h"
#include <QDebug>
static GLchar vertexShaderSource[] =                //顶点着色器
        "#version 330 core \n"                      //用GLSL编写时一定要在每行后加上转义字符，不然会导致编译失败
        "layout(location = 0) in vec3 position;\n"  //它会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。顶点着色器接着会处理我们在内存中指定数量的顶点。
        "out vec4 tofragmentshader;\n"
        "void main(){\n"
           " gl_Position = vec4(position,1.0f);\n"  //我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，它会在GPU内存(通常被称为显存)中储存大量顶点。
            "tofragmentshader = vec4(0.5f,0,0,1.0f);\n"//如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了
        "}\n";
//-------------------------------------------------------------------------------------------------------------------
static GLchar FragmentShaderSource[] =              //片段着色器
        " #version 330 core \n"
        "in vec4 tofragmentshader;\n"
         "out vec4 color; \n"
         "void main(){\n"
            " color = tofragmentshader;\n"
         "}\n";
OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

    setFormat(format);
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::initializeGL()//只执行一次
{
     initializeOpenGLFunctions();

     glClearColor(0.0,0.5,0.5,1.0);

     char *version = (char*)glGetString(GL_VERSION);
     QString result(version);
     qDebug() << result;

    glGenVertexArrays((GLsizei)(VAOsNumber),VAOs);//生成一个VAO对象
    glBindVertexArray(VAOs[Triangle]);//绑定VAO

    GLfloat vertices[VerticesNumber][3] = { //定义这样的顶点数据以后，我们会把它作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器
        { -0.90f, -0.90f ,1.0f}, // Triangle 1
        {  0.85f, -0.90f ,0.0f},
        { -0.90f,  0.85f ,0.0f},
        {  0.90f, -0.85f ,0.0f}, // Triangle 2
        {  0.90f,  0.90f ,0.0f},
        { -0.85f,  0.90f ,0.0f}
    };//顶点模型

    glGenBuffers((GLsizei)(VBOsNumber),&VBO);//生成一个VBO对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO);//绑定VBO
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),(GLvoid*)vertices,GL_STATIC_DRAW);//把顶点数据复制到缓冲的内存里

    QMap<GLenum,GLchar*> shaders;
    shaders[GL_VERTEX_SHADER] = vertexShaderSource;
    shaders[GL_FRAGMENT_SHADER] = FragmentShaderSource;

    ShaderProgram(shaders);

    glVertexAttribPointer(TriangleVerticesIndex,3,GL_FLOAT,TRUE,3*sizeof(GLfloat),(GLvoid*)0);//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    glEnableVertexAttribArray(TriangleVerticesIndex);//允许位置0的属性生效

    glBindVertexArray(0);//解绑VAO，供以后使用，通常情况下当我们配置好OpenGL对象以后要解绑它们，这样我们才不会在其它地方错误地配置它们。
 //   makeCurrent();
}

void OpenGLWindow::resizeGL(int w, int h)//窗口重绘的时候会调用
{
    glViewport(0,0,w,h);
    qDebug() << "glViewport";
}

void OpenGLWindow::paintGL()//调用makeCurrent()/update()时会调用
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[Triangle]);
    glDrawArrays(GL_TRIANGLES,TriangleVerticesIndex,VerticesNumber);//使用当前激活的着色器，之前定义的顶点属性配置，和VBO的顶点数据（通过VAO间接绑定）来绘制图元
  //  glFlush();
    glBindVertexArray(0);
}

void OpenGLWindow::ShaderProgram(QMap<GLenum, GLchar*> program)
{
    QList<GLenum> keys = program.keys();
    for(int i=0;i<program.size();i++)
    {
        if(keys.at(i) == GL_VERTEX_SHADER)//编译顶点着色器
        {
            vertexshader = glCreateShader(GL_VERTEX_SHADER);//创建着色器对象
            glShaderSource(vertexshader,1,&program[GL_VERTEX_SHADER],NULL);//把着色器源代码附加到着色器上
            glCompileShader(vertexshader);//编译着色器
            qDebug() << tr("%1:").arg((unsigned int)vertexshader) << GetShaderCompileError(vertexshader);

        }
        else if(keys.at(i) == GL_FRAGMENT_SHADER)//编译片段着色器
        {
            fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentshader,1,&program[GL_FRAGMENT_SHADER],NULL);
            glCompileShader(fragmentshader);
            qDebug() << tr("%1:").arg((unsigned int)fragmentshader) << GetShaderCompileError(fragmentshader);
        }
    }
    shaderprogram = glCreateProgram();//创建一个着色器程序对象
    glAttachShader(shaderprogram,vertexshader);//将之前编译的着色器附加到程序对象上
    glAttachShader(shaderprogram,fragmentshader);
    glLinkProgram(shaderprogram);//链接程序对象
    qDebug() << tr("%1:").arg((unsigned int)shaderprogram) << GetProgramError(shaderprogram);

    glUseProgram(shaderprogram);//激活这个程序对象，每个着色器调用和渲染调用都会使用这个程序对象

    glDeleteShader(vertexshader);//把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
    glDeleteShader(fragmentshader);

}

QString OpenGLWindow::GetShaderCompileError(GLuint shader)//一定要使用glGetShaderiv和glGetShaderInfoLog来检测GLSL语言编译出来的错误
{                                                         //glGetProgramiv和glGetProgramInfoLog同理
    GLint success;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);//返回success处的值

    GLchar errorinfo[512];//这里使用指针的话会出现错误
    if(!success)
    {
        glGetShaderInfoLog(shader,512,NULL,errorinfo);//返回指定着色器的信息记录
        QString message(errorinfo);
        return message;
    }
    else
    {
        QString message("没有错误");
        return message;
    }

}

QString OpenGLWindow::GetProgramError(GLuint program)
{
    GLint success;
    glGetProgramiv(program,GL_LINK_STATUS,&success);//返回success处的值

    GLchar errorinfo[512];
    if(!success)
    {
        glGetProgramInfoLog(program,512,NULL,errorinfo);//返回指定着色器程序的信息记录
        QString message(errorinfo);
        return message;
    }
    else
    {
        QString message("没有错误");
        return message;
    }
}
