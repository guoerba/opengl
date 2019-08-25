#include "model.h"

Model::Model(const char *objpath, const char *dir, QOpenGLShaderProgram *pro, QObject *parent)
    : QObject(parent)
    , program(pro)    
    , directory(tr(dir))
    , texDiffuseNum(0)
    , texSpecularNum(0)
    , texCount(0)
    , DiffuseNum(0)
    , SpecularNum(0)
{
    loadModel(objpath);
}

Model::Model(QString objpath, QString dir, QOpenGLShaderProgram *pro, QObject *parent)
    : QObject(parent)
    , program(pro)
    , texDiffuseNum(0)
    , texSpecularNum(0)
    , texCount(0)
    , DiffuseNum(0)
    , SpecularNum(0)
    , directory(dir)
{
    if(pro == NULL)
        qDebug() << "no shader program was sent in!";
    else
        loadModel(objpath.toUtf8().constData());
}

Model::~Model()
{

}

void Model::Draw(QOpenGLShaderProgram *pro)
{
    for(int i=0;i<meshes.size();i++)
    {
        meshes[i].Draw(pro);
    }
}
//   我们首先声明了Assimp命名空间内的一个Importer，之后调用了它的ReadFile函数。这个函数需要一个文件路径，它的第二个参数
//是一些后期处理(Post-processing)的选项。除了加载文件之外，Assimp允许我们设定一些选项来强制它对导入的数据做一些额外的计
//算或操作。通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状
//变换为三角形。aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标（你可能还记得我们在纹理教程中说过，在OpenGL中大部分的图
//像的y轴都是反的，所以这个后期处理选项将会修复这个）。
void Model::loadModel(const char *path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);

//   在我们加载了模型之后，我们会检查场景和其根节点不为null，并且检查了它的一个标记(Flag)，来查看返回的数据是不是不完整的。
//如果遇到了任何错误，我们都会通过导入器的GetErrorString函数来报告错误并返回。我们也获取了文件路径的目录路径。
    if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        qDebug() << "ERROR::ASSIMP::" << importer.GetErrorString();
        return;
    }
    qDebug() << "ASSIMP:: Model was loaded successfully! ";

//   初始化QOpenGLTexture   

    testModel(scene->mRootNode,scene);
    qDebug() << "texDiffuseNum: " << texDiffuseNum;
    qDebug() << "texSpecularNum: " << texSpecularNum;
    qDebug() << "totaltexture: " << texSpecularNum + texDiffuseNum;
   // InitializeTextureList(texSpecularNum + texDiffuseNum);
    processNode(scene->mRootNode,scene);
}

//   如果什么错误都没有发生，我们希望处理场景中的所有节点，所以我们将第一个节点（根节点）传入了递归的processNode函数。因为
//每个节点（可能）包含有多个子节点，我们希望首先处理参数中的节点，再继续处理该节点所有的子节点，以此类推。这正符合一个递归结构
//，所以我们将定义一个递归函数。递归函数在做一些处理之后，使用不同的参数递归调用这个函数自身，直到某个条件被满足停止递归。在我
//们的例子中退出条件(Exit Condition)是所有的节点都被处理完毕。
//   Assimp的结构中，每个节点包含了一系列的网格索引，每个索引指向场景对象中的那个特定网格。我们接下来就想去获取这些网格索引
//，获取每个网格，处理每个网格，接着对每个节点的子节点重复这一过程。
void Model::processNode(aiNode *node, const aiScene *scene)
{    
    qDebug() << tr("meshes in node is: %1; node name is: %2")
                .arg(node->mNumMeshes).arg(node->mName.C_Str());
    for(unsigned int i=0;i<node->mNumMeshes;i++)//处理结点的所有网格
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh temp = processMesh(mesh,scene);
        temp.MeshDebug();
        meshes.append(temp);
    }
    for(unsigned int i=0;i<node->mNumChildren;i++)//对子结点重复这一过程
    {
        qDebug() << tr("node name is %1").arg(node->mName.C_Str());
        processNode(node->mChildren[i],scene);        
    }
}

//访问网格的相关属性并将它们储存到我们自己的对象中
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    QVector<MeshVertex> vertices;
    QVector<MeshTexture> textures;
    QVector<unsigned int> indices;

    if(!mesh->HasPositions())
    {
        qDebug() << tr("%1 Mesh's vertices don't have positions!").arg(QString(mesh->mName.C_Str()));
    }
    if(!mesh->HasNormals())
    {
        qDebug() << tr("%1 Mesh's vertices don't have normals!").arg(QString(mesh->mName.C_Str()));
    }
    for(unsigned int i=0;i<mesh->mNumVertices;i++)
    {
        MeshVertex vertex;
        vertex.Position[0] = (mesh->mVertices[i].x);//处理纹理坐标
        vertex.Position[1] = (mesh->mVertices[i].y);
        vertex.Position[2] = (mesh->mVertices[i].z);

        vertex.Normal[0] = (mesh->mNormals[i].x);//处理法线向量
        vertex.Normal[1] = (mesh->mNormals[i].y);
        vertex.Normal[2] = (mesh->mNormals[i].z);

        if(mesh->mTextureCoords[0])// 网格是否有纹理坐标？
        {
            vertex.TexCoords[0] = (mesh->mTextureCoords[0][i].x);//处理纹理坐标
            vertex.TexCoords[1] = (mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.TexCoords[0] = 0.0f;
            vertex.TexCoords[1] = 0.0f;
        }        
        vertices.append(vertex);
    }
    // qDebug() << "vertices: " << vertices.size();

    for(unsigned int i=0;i<mesh->mNumFaces;i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0;j<face.mNumIndices;j++)
        {
            indices.append(face.mIndices[j]);//处理引索
           // qDebug() << tr("execute in processMesh() ,circlue i %1 , circule j %2").arg(i).arg(j);
        }
    }
    //qDebug() << "indices: " << indices.size();

    if(mesh->mMaterialIndex >= 0)//处理材质
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        textures.append(loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse"));
        textures.append(loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular"));
    }
    //qDebug() << "texture: " << textures.size();

    return Mesh(vertices,textures,indices,meshes.size() + 1);
}

//loadMaterialTextures函数遍历了给定纹理类型的所有纹理位置，获取了纹理的文件位置，并加载并和生成了纹理，将信息储存在了一个Vertex结构体中。
QVector<MeshTexture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, QString typeName)
{
    QVector<MeshTexture> textures;
    for(unsigned int i=0;i<material->GetTextureCount(type);i++)
    {
        aiString str;
        material->GetTexture(type,i,&str);
        if(str.length != 0)
        {
            textureList.append(new QOpenGLTexture(QOpenGLTexture::Target2D));
            unsigned int texsize = textureList.size();

         /*   QString uName;
            if(typeName == "texture_diffuse")
            {
                uName = tr("texture_diffuse%1").arg(DiffuseNum);
                qDebug() << "textureSampler: " << uName;
                DiffuseNum++;
            }
            else if(typeName == "texture_specular")
            {
                uName = tr("texture_specular%1").arg(SpecularNum);
                qDebug() << "textureSampler: " << uName;
                SpecularNum++;
            }*/
          //  qDebug() << tr("execute in processMesh() ,DiffuseNum is %1 , SpecularNum is %2").arg(DiffuseNum).arg(SpecularNum);
            MeshTexture tex;
          //  SetTexture(textureList[texsize - 1], texsize - 1,uName,QString(str.C_Str()));
            SetTexture(textureList[texsize - 1], texsize - 1,QString(str.C_Str()));
            tex.texture = textureList[texsize - 1];
            tex.sampler = texsize - 1;
            tex.type = typeName;
            tex.path = QString(str.C_Str());
            textures.append(tex);
        }
    }       
    return textures;
}

GLuint Model::SetTexture(QOpenGLTexture *tex, GLuint Sampler, QString path)
{
    tex->create();//glGenTextures(1, &texture);
    tex->bind(Sampler);//glActiveTexture(GL_TEXTURE0 + Sampler);glBindTexture(GL_TEXTURE_2D, texture->ID());   
    tex->setMagnificationFilter(QOpenGLTexture::Linear);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    tex->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    tex->setData(QImage(directory + '/' + path));//glTexSubImage2D
    tex->release(Sampler);
    return tex->textureId();
}

void Model::testModel(aiNode *node, const aiScene *scene)
{
    qDebug() << "-------------------------------------------------------------------------";
    qDebug() << tr("meshes in node is: %1; node name is: %2")
                .arg(node->mNumMeshes).arg(node->mName.C_Str());
    for(unsigned int i=0;i<node->mNumMeshes;i++)//处理结点的所有网格
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        qDebug() << tr("node->mMeshes[%1]: %2").arg(i).arg(node->mMeshes[i]);
        qDebug() << tr("mesh %1 has %2 vertices,%3 faces")
                    .arg(mesh->mName.C_Str()).arg(mesh->mNumVertices).arg(mesh->mNumFaces);
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        for(unsigned int j=0;j<material->GetTextureCount(aiTextureType_DIFFUSE);j++)
        {
            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE,i,&str);
            qDebug() << tr("mesh's DIFFUSE material is: \" %1 \",which length is %2").arg(str.C_Str()).arg(str.length);
            if(str.length != 0)
                texDiffuseNum++;
        }
        for(unsigned int j=0;j<material->GetTextureCount(aiTextureType_DIFFUSE);j++)
        {
            aiString str;
            material->GetTexture(aiTextureType_SPECULAR,i,&str);
            qDebug() << tr("mesh's SPECULAR material is: \" %1 \",which length is %2").arg(str.C_Str()).arg(str.length);
            if(str.length != 0)
                texSpecularNum++;
        }
        qDebug() << "-------------------------------------------------------------------------";
    }
    for(unsigned int i=0;i<node->mNumChildren;i++)//对子结点重复这一过程
        testModel(node->mChildren[i],scene);
}
