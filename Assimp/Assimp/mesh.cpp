#include "mesh.h"

Mesh::Mesh(QVector<MeshVertex> vertices,QVector<MeshTexture> textures,QVector<unsigned int> indices,int sign)
    : vertices(vertices)
    , textures(textures)
    , indices(indices)
    , sign(sign)
    , VAO(0)
    , VBO(0)
    , EBO(0)

{
    SetupMesh();
}

Mesh::Mesh(const Mesh &m)//浅拷贝
{
    this->vertices = m.vertices;
    this->textures = m.textures;
    this->indices = m.indices;    
    this->VAO = m.VAO;
    this->VBO = m.VBO;
    this->EBO = m.EBO;
}

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::Draw(QOpenGLShaderProgram *program)
{
    initializeOpenGLFunctions();

    /*foreach(MeshTexture texture,textures)
    {
        texture.texture->bind(texture.sampler);
    }*/

    for(int i=0;i<textures.size();i++)
    {
        QString name = textures[i].type;
        if(name == QString("texture_diffuse"))
        {
            program->setUniformValue(QString("texture_diffuse%1").arg(sign).toUtf8().constData(),
                                     static_cast<GLuint>(textures[i].sampler));

        }
        else if(name == QString("texture_specular"))
        {
            program->setUniformValue(QString("texture_specular%1").arg(sign).toUtf8().constData(),
                                     static_cast<GLuint>(textures[i].sampler));
        }

        textures[i].texture->bind(i);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    initializeOpenGLFunctions();

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(MeshVertex),reinterpret_cast<void*>(vertices.data()),GL_STATIC_DRAW);
    MeshVertex *data = vertices.data();
    for(int i=0;i<vertices.size();i++)
    {
       /* qDebug() << "vertex pos(location):      " << &(data[i].Position[0]) << "(data.x): " << data[i].Position[0];
        qDebug() << "vertex pos(location):      " << &(data[i].Position[1]) << "(data.y): " << data[i].Position[1];
        qDebug() << "vertex pos(location):      " << &(data[i].Position[2]) << "(data.z): " << data[i].Position[2];
        qDebug() << "vertex normal(location):   " << &(data[i].Normal[0]) << "(data.x): " << data[i].Normal[0];
        qDebug() << "vertex normal(location):   " << &(data[i].Normal[1]) << "(data.y): " << data[i].Normal[1];
        qDebug() << "vertex normal(location):   " << &(data[i].Normal[2]) << "(data.z): " << data[i].Normal[2];
        qDebug() << "vertex texcoord(location): " << &(data[i].TexCoords[0]) << "(data.x): " << data[i].TexCoords[0];
        qDebug() << "vertex texcoord(location): " << &(data[i].TexCoords[1]) << "(data.y): " << data[i].TexCoords[1];
        qDebug() << "vertex textures coordinate: " << "(" << data[i].TexCoords[0] << "," << data[i].TexCoords[1] << ")";*/
    }

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),reinterpret_cast<void*>(indices.data()),GL_STATIC_DRAW);

    qDebug() << "VAO is: " << VAO;
    qDebug() << "VBO is: " << VBO;
    qDebug() << "EBO is: " << EBO;
    qDebug() << "sign is: " << sign;

    glEnableVertexAttribArray(0);    
    glVertexAttribPointer(0,3,GL_FLOAT,GL_TRUE,sizeof(MeshVertex),reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_TRUE,sizeof(MeshVertex),reinterpret_cast<void*>(offsetof(MeshVertex,Normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_TRUE,sizeof(MeshVertex),reinterpret_cast<void*>(offsetof(MeshVertex,TexCoords)));

    glBindVertexArray(0);
}

void Mesh::MeshDebug()
{
    /*for(int i=0;i<vertices.size();i++)
    {
        qDebug() << "Mesh::MeshVertex.Position: " << vertices[i].Position
                 << ";;Mesh::MeshVertex.Normal: " << vertices[i].Normal
                 << ";;Mesh::MeshVertex.Texcoords: " << vertices[i].TexCoords;
    }*/

    for(int i=0;i<textures.size();i++)
    {
        qDebug() << "Mesh::MeshTexture.sampler: " << textures[i].sampler <<
                    ";;Mesh::MeshTexture.type: " << textures[i].type <<
                    ";;Mesh::MeshTexture.path: " << textures[i].path;
    }

    /*for(int i=0;i<indices.size();i++)
    {
        qDebug() << "Mesh::Index: " << indices[i];
    }*/
    qDebug() << "Mesh::Index: " << indices.size();

}
