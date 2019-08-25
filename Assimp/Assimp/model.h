#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <QOpenGLTexture>

class Model : public QObject
{
    Q_OBJECT
public:
    Model(const char *objpath, const char *dir,QOpenGLShaderProgram *pro = NULL, QObject *parent = 0);
    Model(QString objpath, QString dir,QOpenGLShaderProgram *pro = NULL, QObject *parent = 0);    
    ~Model();
    void Draw(QOpenGLShaderProgram *pro = NULL);
signals:

public slots:
private:
    QVector<Mesh> meshes;
    QVector<QOpenGLTexture*> textureList;
    QOpenGLShaderProgram *program;
    QString directory;

    void loadModel(const char *path);
    void processNode(aiNode *node,const aiScene *scene);
    Mesh processMesh(aiMesh *mesh,const aiScene *scene);
    QVector<MeshTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QString typeName);
    GLuint SetTexture(QOpenGLTexture *tex, GLuint Sampler, QString path);
    void testModel(aiNode *node,const aiScene *scene);

    unsigned int texDiffuseNum,texSpecularNum,texCount,DiffuseNum,SpecularNum;
};

#endif // MODEL_H
