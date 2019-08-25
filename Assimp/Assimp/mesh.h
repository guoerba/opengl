#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QDebug>

//   C++结构体有一个很棒的特性，它们的内存布局是连续的(Sequential)。也就是说，如果我们将结构体作为一个数据数组使用，
//那么它将会以顺序排列结构体的变量，这将会直接转换为我们在数组缓冲中所需要的float（实际上是字节）数组。
typedef struct{
    float Position[3];//顶点位置
    float Normal[3];//顶点所在面法向量
    float TexCoords[2];//纹理坐标
}MeshVertex;
//   结构体的另外一个很好的用途是它的预处理指令offsetof(s, m)，它的第一个参数是一个结构体，第二个参数是这个结构体中
//变量的名字。这个宏会返回那个变量距结构体头部的字节偏移量(Byte Offset)。这正好可以用在定义glVertexAttribPointer
//函数中的偏移参数
typedef struct{
    QOpenGLTexture *texture;//纹理id
    QString type;//纹理类型（漫反射贴图/镜面反射贴图）
    QString path;//
    unsigned int sampler;
    float shineness;//光照散射系数
}MeshTexture;


class Mesh : protected QOpenGLFunctions_4_5_Core
{
public:
    explicit Mesh();
    Mesh(QVector<MeshVertex> vertices, QVector<MeshTexture> textures, QVector<unsigned int> indices, int sign);
    Mesh(const Mesh &m);
    ~Mesh();
    void Draw(QOpenGLShaderProgram *program = NULL);
    void MeshDebug();

    QVector<MeshVertex> vertices;
    QVector<MeshTexture> textures;
    QVector<unsigned int> indices;
signals:

public slots:
private:    
    unsigned int VAO,VBO,EBO;
    int sign;

    void SetupMesh();    
};

#endif // MESH_H
