#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , VAO(0)
    , VBO(0)
    , EBO(0)
    , vnumber(0)
    , count(0)    
    , refreshFlag(false)
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
}

OpenGLWidget::~OpenGLWidget()
{
    if(program->isLinked())
        program->destroyed(this);
    if(programforSphere->isLinked())
        programforSphere->destroyed(this);
}

void OpenGLWidget::ReceiveData(QVector3D location, QVector4D rotate, QVector3D scale, QString dir, QString path)
{
    vectorModel.append(new Model(path,dir,program,this));//新的模型对象
    vectorTranslate.append(location);//新模型的世界坐标位置
    vectorRotate.append(rotate);//新模型的世界坐标旋转
    vectorScale.append(scale);//新模型的世界坐标缩放
    refreshFlag = true;
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

    ShaderProgram(tr(":/shader/vertexshader.txt"),tr(":/shader/fragmentshader.txt"),program);    

    SetSphereVertex();
    glEnable(GL_DEPTH_TEST);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(1);//100fps
}

void OpenGLWidget::resizeGL(int w, int h)
{
    SetProjectionMatrix(w,h,program);
    SetProjectionMatrix(w,h,programforSphere);
}

void OpenGLWidget::paintGL()
{        
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    for(int i=0;i<vectorModel.size();i++)
    {
        qDebug() << "vectorModel.size: " << vectorModel.size();
        glUseProgram(program->programId());
        SetModelMatrix(i,program);
        SetViewMatrix(program);
        SetProjectionMatrix(width(),height(),program);
        vectorModel[i]->Draw(program);
    }

    glUseProgram(programforSphere->programId());
    glBindVertexArray(VAO_Sphere);
    SetModelMatrix(0,programforSphere);
    SetViewMatrix(programforSphere);
    SetProjectionMatrix(width(),height(),programforSphere);
    glDrawElements(GL_TRIANGLES,vnumber,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    if(count >= 360)
        count = 0;
    count++;
}

void OpenGLWidget::ShaderProgram(const char* vshader,
                                 const char* fshader,
                                 QOpenGLShaderProgram *pro)
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

void OpenGLWidget::ShaderProgram(const QString &vshaderfileName,
                                 const QString &fshaderfileName,
                                 QOpenGLShaderProgram *pro)
{
    if(!pro->addShaderFromSourceFile(QOpenGLShader::Vertex,vshaderfileName))
        qDebug() << tr("error: ") << pro->log();
    else
        qDebug() << tr("VertexShader was successfully complied!");

    if(!pro->addShaderFromSourceFile(QOpenGLShader::Fragment,fshaderfileName))
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
        if(pro == program)
        {            
            SpotLight light;
            light.ambient = 0.1f;
            light.diffuse = 0.5f;
            light.specular = 1.0f;
            light.cutoff = cos(radian(12.5));
            light.outercutoff = cos(radian(25.0));
            light.position = viewcamera->CameraPos();
            light.spotdir = viewcamera->CameraFront();
            light.light = lightcolor;
            SetLight(light,pro);                 
        }
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
    if(pro == programforSphere)
    {
        model.translate(lightTranslate);
        model.scale(cubeScale[0][0],cubeScale[0][1],cubeScale[0][2]);
    }
    else
    {
        model.translate(vectorTranslate[index]);
        model.rotate(vectorRotate[index].w(),
                     vectorRotate[index].x(),
                     vectorRotate[index].y(),
                     vectorRotate[index].z());
        model.scale(vectorScale[index]);
    }
    pro->setUniformValue(pro->uniformLocation("model"),model);//uniform参数是和不同着色器程序相关的   
    pro->setUniformValue(pro->uniformLocation("lightcolor"),lightcolor);
    if(pro != programforSphere)
    {
        pro->setUniformValue("lightPos",lightTranslate);
//正规矩阵是用来解决法向量在不等比缩放中的畸变映射，计算方法是：模型矩阵的逆矩阵的转置矩阵的左上角3x3矩阵
        pro->setUniformValue("normalModelMat",model.normalMatrix());//model.normalMatrix() = model.inverted().transpose()
        if(pro == program)
        {            
            PointLight light;
            light.position = QVector3D(cubePosition[0][0],cubePosition[0][1],cubePosition[0][2]);
            light.light = lightcolor;
            light.ambient = 0.1f;
            light.diffuse = 0.5f;
            light.specular = 1.0f;
            light.constant = 1.0f;
            light.linear = 0.14f;
            light.quadratic = 0.07f;
            SetLight(light,pro);

            DirectionalLight directionlight;
            directionlight.ambient = 0.1f;
            directionlight.diffuse = 1.0f;
            directionlight.specular = 1.0f;
            directionlight.direction = QVector3D(0.0f, 1.0f, -1.0f);
            directionlight.light = lightcolor;
            SetLight(directionlight,pro);
        }
    }
    else
    {
    }
}

void OpenGLWidget::SetTexture(QOpenGLTexture *tex,GLuint Sampler,QOpenGLShaderProgram *pro,const char* name,QImage img)
{    
    tex->create();//glGenTextures(1, &texture);
    tex->bind(Sampler);//glActiveTexture(GL_TEXTURE0 + Sampler);glBindTexture(GL_TEXTURE_2D, texture->ID());
    pro->setUniformValue(name,Sampler);//glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
    tex->setMagnificationFilter(QOpenGLTexture::Linear);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::ClampToBorder);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    tex->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::ClampToBorder);
    tex->setData(img.mirrored());//glTexSubImage2D
    tex->release(Sampler);
}

void OpenGLWidget::SetMaterial(Material material,QOpenGLShaderProgram *pro)
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

void OpenGLWidget::SetMaterial(TexMaterial texmaterial, QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable program is found;";
        return;
    }
    pro->setUniformValue("material.diffuse",texmaterial.diffuse);
    pro->setUniformValue("material.specular",texmaterial.specular);
    pro->setUniformValue("material.shineness",texmaterial.shineness);
}

void OpenGLWidget::SetLight(Light light, QOpenGLShaderProgram *pro)
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

void OpenGLWidget::SetLight(DirectionalLight light, QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable directional light is found;";
        return;
    }
    pro->setUniformValue("directionallight.ambient",light.ambient);
    pro->setUniformValue("directionallight.diffuse",light.diffuse);
    pro->setUniformValue("directionallight.specular",light.specular);
    pro->setUniformValue("directionallight.direction",light.direction);
    pro->setUniformValue("directionallight.light",light.light);
}

void OpenGLWidget::SetLight(PointLight light, QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable directional light is found;";
        return;
    }
    pro->setUniformValue("pointlight.position",light.position);
    pro->setUniformValue("pointlight.light",light.light);
    pro->setUniformValue("pointlight.ambient",light.ambient);
    pro->setUniformValue("pointlight.diffuse",light.diffuse);
    pro->setUniformValue("pointlight.specular",light.specular);
    pro->setUniformValue("pointlight.constant",light.constant);
    pro->setUniformValue("pointlight.linear",light.linear);
    pro->setUniformValue("pointlight.quadratic",light.quadratic);
}

void OpenGLWidget::SetLight(SpotLight light, QOpenGLShaderProgram *pro)
{
    if(pro == NULL)
    {
        qDebug() << "No suitable directional light is found;";
        return;
    }
    if(light.outercutoff > light.cutoff)
        light.outercutoff = light.cutoff;
    pro->setUniformValue("spotlight.position",light.position);
    pro->setUniformValue("spotlight.spotdir",light.spotdir);
    pro->setUniformValue("spotlight.cutoff",light.cutoff);
    pro->setUniformValue("spotlight.outercutoff",light.outercutoff);
    pro->setUniformValue("spotlight.ambient",light.ambient);
    pro->setUniformValue("spotlight.diffuse",light.diffuse);
    pro->setUniformValue("spotlight.specular",light.specular);
    pro->setUniformValue("spotlight.light",light.light);
}

void OpenGLWidget::RefreshModel()
{

}
