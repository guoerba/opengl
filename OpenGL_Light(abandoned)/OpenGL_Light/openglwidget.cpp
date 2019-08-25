#include "openglwidget.h"
static const char vertexshadersource[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec3 vertex;\n"
    "layout(location = 1) in vec3 tocolor;\n"
    "layout(location = 2) in vec2 coord;\n "
    "out vec2 TexCoord;\n "
    "uniform mat4 model;\n "//变换矩阵;
    "uniform mat4 view;\n "
    "uniform mat4 projection;\n "
    "void main(){\n"
    "gl_Position = projection*view*model*vec4(vertex,1.0f); \n"
    "TexCoord = coord;\n "
    "}\n "
};

static const char fragmentshadersource[] = {
  "#version 450 core\n"
    "in vec2 TexCoord;\n "
    "out vec4 color;\n"
    "uniform sampler2D Tex1;\n "
    "uniform sampler2D Tex2;\n "
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
    //一个拥有焦点(focus)的QWidget才可以接受键盘事件。有输入焦点的窗口是活动窗口或活动窗口子窗口或子子窗口等。
    //Qt::TabFocus 通过Tab键获得焦点
    //Qt::ClickFocus 通过被单击获得焦点
    //Qt::StrongFocus 可通过上面两种方式获得焦点
    //Qt::NoFocus 不能通过上两种方式获得焦点(默认值),setFocus仍可使其获得焦点
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

OpenGLWidget::~OpenGLWidget()
{
    program->destroyed();
    texture[0]->destroy();
    texture[1]->destroy();

}

void OpenGLWidget::SetSphereVertex()
{
    glGenVertexArrays(1,&VAO[1]);
    glBindVertexArray(VAO[1]);

    Sphere sphere(2,this);

    sphere.GetSphere();
    QVector<float> spherevertices = sphere.outputVertices();
    int vlen = sphere.outputVerticesLen();
    float *head = spherevertices.data();
    QVector<int> sphereverticesorder = sphere.outputOrder();
    int olen = sphere.outputOrderLen();
    int *inthead = sphereverticesorder.data();

  /* qDebug() << "spherevertices: " << spherevertices;
    qDebug() << "vlen: " << vlen;

    for(int i=0;i<(vlen/sizeof(float));i++)
        qDebug() << tr("output %1: ").arg(i) << head[i];
    qDebug() << "sphereverticesorder: " << sphereverticesorder;
    qDebug() << "olen: " << olen;

    for(int i=0;i<(olen/sizeof(int));i++)
        qDebug() << tr("output %1: ").arg(i) << inthead[i];*/

    glGenBuffers(2,&VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,vlen,head,GL_STATIC_DRAW);


    glGenBuffers(1,&EBO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,olen,inthead,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,FALSE,3*sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}


void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.5f,1.0f,1.0f,1.0f);

    glGenVertexArrays(1,&VAO[0]);
    glBindVertexArray(VAO[0]);

    glGenBuffers(1,&VBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenBuffers(1,&EBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    ShaderProgram();

    SetTexture();//设置纹理
    glVertexAttribPointer(0,3,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,FALSE,8*sizeof(GLfloat),(void*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width(),height());
    glBindVertexArray(0);

    SetSphereVertex();

    viewcamera = new Camera(this,QVector3D(0,0,-15.0),QVector3D(0,0,0),QVector3D(0,1,0));

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(10);//100fps

}

void OpenGLWidget::resizeGL(int w, int h)
{
    //glViewport(0,0,w,h);//视口变换将位于-1.0到1.0范围的坐标变换到由glViewport函数所定义的坐标范围内。
    SetProjectionMatrix(w,h);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    texture[0]->bind(0);
    texture[1]->bind(1);
    glBindVertexArray(VAO[0]);
    for(int i=0;i<10;i++)
    {
        SetModelMatrix(i);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,NULL);
    }
    glBindVertexArray(0);    

    glBindVertexArray(VAO[1]);
    SetModelMatrix(10);
    glDrawElements(GL_TRIANGLES,32,GL_UNSIGNED_INT,NULL);
    glBindVertexArray(0);
    texture[0]->release(0);
    texture[1]->release(1);

    SetViewMatrix();
    SetProjectionMatrix(width(),height());

    if(count >= 360)
        count = 0;
    count += 1;

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

//当矩阵相乘时，在最右边的矩阵是第一个与向量相乘的，所以你应该从右向左读这个乘法。
//在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，否则它们会（消极地）互相影响。
void OpenGLWidget::SetProjectionMatrix(int width,int height)
{
    QMatrix4x4 projection;
//perspective用于透视投影，功能和glFrustum非常相似，参数不一样。相对于glFrustum来说不常用。
//当视口(viewport)的纵横比和视景体(frustum)的纵横比相同的时候，改变窗口大小，图像才不会变形；
    projection.perspective(45.0f + viewcamera->doscale(),((float)width)/((float)height),0.1f,100.0f);//投影矩阵（透视投影）
    glViewport(0,0,width,height);
    program->setUniformValue(program->uniformLocation("projection"),projection);
}

void OpenGLWidget::SetViewMatrix()//观察矩阵,参数：(摄像机位置，被观察对象中心位置，相对于摄像机位置的上向量)
{
    viewcamera->domovement();    
    QMatrix4x4 view;
    view.lookAt(viewcamera->CameraPos(),viewcamera->CameraPos()+viewcamera->CameraFront()
                ,viewcamera->CameraUp());
    program->setUniformValue(program->uniformLocation("view"),view);

}

void OpenGLWidget::SetModelMatrix(int index) //模型矩阵
{
    QMatrix4x4 model;
    model.translate(cubePosition[index][0],cubePosition[index][1],cubePosition[index][2]);
    model.rotate((float)(-count),0.5f,1.0f,0.0f+((float)index)/10.0);
    program->setUniformValue(program->uniformLocation("model"),model);
}

