#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
//我们为每个冯氏光照模型的元素都定义一个颜色向量。
//ambient材质向量定义了在环境光照下这个物体反射的是什么颜色,通常这是和物体颜色相同的颜色。
//diffuse材质向量定义了在漫反射光照下物体的颜色,漫反射颜色被设置为(和环境光照一样)我们需要的物体颜色。
//specular材质向量设置的是物体受到的镜面光照的影响的颜色(或者可能是反射一个物体特定的镜面高光颜色)。
//最后，shininess影响镜面高光的散射/半径。
typedef struct{
    QVector3D ambient;//环境光材质
    QVector3D diffuse;//漫反射材质
    QVector3D specular;//镜面反射材质
    float shineness;//光照散射系数（越低越散）
}Material;

typedef struct{
    QVector3D light;//光照
    float ambient;//环境光照强度
    float diffuse;//漫反射强度
    float specular;//镜面反射强度
}Light;

#endif // MATERIAL_H
