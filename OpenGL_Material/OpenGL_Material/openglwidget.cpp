#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , VAO(0)
    , VBO(0)
    , EBO(0)
    , VAO_Sphere(0)
    , VBO_Sphere(0)
    , EBO_Sphere(0)
    , vnumber(0)
    , count(0)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,5);

    this->setFormat(format);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    viewcamera = new Camera(this,QVector3D(0,0,-15.0),QVector3D(0,0,0),QVector3D(0,1,0));
    program = new QOpenGLShaderProgram(this);
    programforSphere = new QOpenGLShaderProgram(this);
    programforUntexture = new QOpenGLShaderProgram(this);
}

OpenGLWidget::~OpenGLWidget()
{
    program->destroyed(this);
    programforSphere->destroyed(this);
    programforUntexture->destroyed(this);
}

void OpenGLWidget::SetUntextureVertex()
{
    glGenVertexArrays(1,&VAO_Untexture);
    glBindVertexArray(VAO_Untexture);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    ShaderProgram(vertexShaderforUntexture,fragmentShaderforUntexture,programforUntexture);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),reinterpret_cast<void*>(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),reinterpret_cast<void*>(8*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


}

void OpenGLWidget::SetSphereVertex()//球顶点
{
    glGenVertexArrays(1,&VAO_Sphere);
    glBindVertexArray(VAO_Sphere);

    Sphere sphere(5,this);

    sphere.GetSphere();
    QVector<float> spherevertices = sphere.outputVertices();
    int vlen = sphere.outputVerticesLen();
    float *head = spherevertices.data();
    QVector<int> sphereverticesorder = sphere.outputOrder();
    int olen = sphere.outputOrderLen();
    int *inthead = sphereverticesorder.data();
    vnumber = olen;

  /*  qDebug() << "spherevertices: " << spherevertices;
    qDebug() << "vlen: " << vlen;

    for(int i=0;i<(vlen/sizeof(float));i++)
        qDebug() << tr("output %1: ").arg(i) << head[i];
    qDebug() << "sphereverticesorder: " << sphereverticesorder;
    qDebug() << "olen: " << olen;

    for(int i=0;i<(olen/sizeof(int));i++)
        qDebug() << tr("output %1: ").arg(i) << inthead[i];*/

    glGenBuffers(2,&VBO_Sphere);
    glBindBuffer(GL_ARRAY_BUFFER,VBO_Sphere);
    glBufferData(GL_ARRAY_BUFFER,vlen,reinterpret_cast<void*>(head),GL_STATIC_DRAW);

    glGenBuffers(1,&EBO_Sphere);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO_Sphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,olen,reinterpret_cast<void*>(inthead),GL_STATIC_DRAW);

    ShaderProgram(vertexShaderforSphere,fragmentShaderforSphere,programforSphere);

    glVertexAttribPointer(0,3,GL_FLOAT,FALSE,3*sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    ShaderProgram(vertexShader,fragmentShader,program);

    SetTexture();

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),reinterpret_cast<void*>(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),reinterpret_cast<void*>(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,11*sizeof(GLfloat),reinterpret_cast<void*>(8*sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width(),height());
    glBindVertexArray(0);

    SetSphereVertex();
    SetUntextureVertex();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1);//100fps
}

void OpenGLWidget::resizeGL(int w, int h)
{
    SetProjectionMatrix(w,h,program);
    SetProjectionMatrix(w,h,programforSphere);
    SetProjectionMatrix(w,h,programforUntexture);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(program->programId());
//先绑定两个纹理到对应的纹理单元，然后定义哪个uniform采样器对应哪个纹理单元：
    texture[0]->bind(0);
    program->setUniformValue("Tex1",0);
    texture[1]->bind(1);
    program->setUniformValue("Tex2",1);

    glBindVertexArray(VAO);
    SetModelMatrix(1,program);
    SetViewMatrix(program);
    SetProjectionMatrix(width(),height(),program);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    texture[0]->release(0);
    texture[1]->release(1);

    glUseProgram(programforSphere->programId());
    glBindVertexArray(VAO_Sphere);
    SetModelMatrix(0,programforSphere);
    SetViewMatrix(programforSphere);
    SetProjectionMatrix(width(),height(),programforSphere);
    glDrawElements(GL_TRIANGLES,vnumber,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    glUseProgram(programforUntexture->programId());
    glBindVertexArray(VAO_Untexture);
    SetModelMatrix(2,programforUntexture);
    SetViewMatrix(programforUntexture);
    SetProjectionMatrix(width(),height(),programforUntexture);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);   

    if(count >= 360)
        count = 0;
    count++;
}

void OpenGLWidget::ShaderProgram(const char* vshader, const char* fshader, QOpenGLShaderProgram *pro)
{
    if(!pro->addShaderFromSourceCode(QOpenGLShader::Vertex,vshader))
        qDebug() << tr("error: ") << pro->log();
    else
        qDebug() << tr("VertexShader was successfully complied!");

    if(!pro->addShaderFromSourceCode(QOpenGLShader::Fragment,fshader))
        qDebug() << tr("error: ") << pro->log();
    else
        qDebug() << tr("FragmentShader was successfully complied!");

    if(!pro->link())
        qDebug() << tr("error: ") << pro->log();
    else
        qDebug() << tr("Shaders were successfully linked!");
}

void OpenGLWidget::SetProjectionMatrix(int width, int height, QOpenGLShaderProgram *pro)
{
    QMatrix4x4 projection;
//perspective用于透视投影，功能和glFrustum非常相似，参数不一样。相对于glFrustum来说不常用。
//当视口(viewport)的纵横比和视景体(frustum)的纵横比相同的时候，改变窗口大小，图像才不会变形；
    projection.perspective(45.0f + viewcamera->doscale(),((float)width)/((float)height),0.1f,100.0f);//投影矩阵（透视投影）
    glViewport(0,0,width,height);
    pro->setUniformValue(pro->uniformLocation("projection"),projection);
}

void OpenGLWidget::SetViewMatrix(QOpenGLShaderProgram *pro)//观察矩阵,参数：(摄像机位置，被观察对象中心位置，相对于摄像机位置的上向量)
{
    viewcamera->domovement();
    QMatrix4x4 view;
    view.lookAt(viewcamera->CameraPos(),viewcamera->CameraPos()+viewcamera->CameraFront()
                ,viewcamera->CameraUp());
    pro->setUniformValue(pro->uniformLocation("view"),view);
    if(pro != programforSphere)
    {       
        pro->setUniformValue("normalViewMat",view.normalMatrix());      
    }
    else
    {

    }
}

void OpenGLWidget::SetModelMatrix(int index, QOpenGLShaderProgram *pro) //模型矩阵
{
    QMatrix4x4 model;
    QVector3D lightTranslate(cubePosition[0][0],cubePosition[0][1],cubePosition[0][2]);
   // QVector3D lightTranslate(10*sin(radian(count))*sin(radian(count)),10*cos(radian(count)),10*sin(radian(count))*cos(radian(count)));
    switch(index)
    {
    case 0:
        model.translate(lightTranslate);
        model.scale(cubeScale[index][0],cubeScale[index][1],cubeScale[index][2]);
        break;
    default:
        model.translate(cubePosition[index][0],cubePosition[index][1],cubePosition[index][2]);
        model.scale(cubeScale[index][0],cubeScale[index][1],cubeScale[index][2]);
        break;
    }
    pro->setUniformValue(pro->uniformLocation("model"),model);//uniform参数是和不同着色器程序相关的   
    pro->setUniformValue(pro->uniformLocation("lightcolor"),lightcolor);
    if(pro != programforSphere)
    {
        pro->setUniformValue("lightPos",lightTranslate);
//正规矩阵是用来解决法向量在不等比缩放中的畸变映射，计算方法是：模型矩阵的逆矩阵的转置矩阵的左上角3x3矩阵
        pro->setUniformValue("normalModelMat",model.normalMatrix());//model.normalMatrix() = model.inverted().transpose()
        if(pro == programforUntexture)
        {
            Material material;
            material.ambient = QVector3D(1.0f, 0.5f, 0.31f);
            material.diffuse = QVector3D(1.0f, 0.5f, 0.31f);
            material.specular = QVector3D(0.5f, 0.5f, 0.5f);
            material.shineness = 32.0f;
            SetMetaMaterial(material,pro);

            Light light;
            light.ambient = 0.1f;
            light.diffuse = 0.5f;
            light.specular = 1.0f;
            light.light = lightcolor;
            SetLightStrength(light,pro);
        }
    }
    else
    {
    }
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
    texture[0]->setData(QImage("E:/QT/QTproject/OpenGL/OpenGL_Light/Light/box.jpg"));//glTexSubImage2D
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
    texture[1]->setData(QImage("E:/QT/QTproject/OpenGL/OpenGL_Light/Light/awesomeface.png").mirrored());
    texture[1]->release(1);
}

void OpenGLWidget::SetMetaMaterial(Material material,QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable program is found;";
        return;
    }
    pro->setUniformValue("material.ambient",material.ambient);
    pro->setUniformValue("material.diffuse",material.diffuse);
    pro->setUniformValue("material.specular",material.specular);
    pro->setUniformValue("material.shineness",material.shineness);
}

void OpenGLWidget::SetLightStrength(Light light, QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable light is found;";
        return;
    }
    pro->setUniformValue("light.ambient",light.ambient);
    pro->setUniformValue("light.diffuse",light.diffuse);
    pro->setUniformValue("light.specular",light.specular);
    pro->setUniformValue("light.light",light.light);
}
