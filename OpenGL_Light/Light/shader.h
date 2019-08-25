﻿#ifndef SHADER_H
#define SHADER_H
/*这里有一个技术点是使用两个不同的矩阵来变换本地坐标提供的同一个顶点位置，并将结果独立的传递给片段着色器。
经过完整的变换（world-view-projection变换）后结果传递给系统变量’gl_Position’，
然后GPU负责将它变换到屏幕空间坐标系并用来进行实际的光栅化操作。
局部变换到世界空间的结果传给了一个用户自定义的属性，这个属性在光栅化阶段被进行了简单的插值，
所以片段着色器中激活的每一个像素都会提供它自己的世界空间位置坐标。这种技术很普遍也很有用。*/
const static char vertexShader[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec3 vertex;\n "
    "layout(location = 1) in vec3 incolor;\n "
    "layout(location = 2) in vec2 coord;\n"
    "layout(location = 3) in vec3 normal;\n "    
    "out vec2 Texcoord; \n"
    "out vec3 outcolor;\n "
    "out vec3 vertexPos;\n "//顶点在世界坐标中的位置向量
    "out vec3 Normal;\n "//未处理的法向量
    "out mat4 transformMat;\n "
    "uniform mat4 model;\n "//变换矩阵;
    "uniform mat4 view;\n "
    "uniform mat4 projection;\n "
    "uniform mat3 normalModelMat;\n "//正规矩阵
    "uniform mat3 normalViewMat;\n "
    "void main(){\n "
        "gl_Position = projection*view*model*vec4(vertex,1.0f);\n "
        "vertexPos = vec3(view*model*vec4(vertex,1.0f));\n "
        "transformMat = view;\n "
        "outcolor = incolor;\n "
        "Texcoord = coord;\n "
        "Normal = normalViewMat*normalModelMat*normal;\n "
    "}\n"
};
//当我们把光源的颜色（向量）与物体的颜色（向量）相乘（各分量相乘），所得到的就是这个物体所反射该光源的颜色（向量）(也就是我们感知到的颜色)。
//环境光照：用光的颜色乘以一个(数值)很小常量环境因子，再乘以物体的颜色，然后使用它作为片段的颜色
const static char fragmentShader[] = {
    "#version 450 core\n"
    "in vec2 Texcoord;\n "
    "in vec3 outcolor;\n "    
    "in vec3 vertexPos;\n "//顶点在观察坐标中的位置向量
    "in vec3 Normal;\n "//未处理的法向量
    "in mat4 transformMat;\n "
    "out vec4 color;\n "
    "uniform sampler2D Tex1;\n "//纹理0
    "uniform sampler2D Tex2;\n "//纹理1
    "uniform vec3 lightcolor;\n "//灯光颜色
    "uniform vec3 lightPos;\n "//灯光位置
    "uniform vec3 viewPos;\n "//观察者位置

    "void main(){\n"
        "vec3 lightDir = vec3(transformMat*vec4(lightPos,1.0f));\n "

        "float ambientStrength = 0.1f;\n "//环境因子
        "vec3 ambient = ambientStrength*lightcolor;\n "//环境光照

        "vec3 intrudelight = normalize(lightDir - vertexPos);\n "//入射光（方向向量）
        "vec3 norm = normalize(Normal);\n "
        "float diffuseStrength = max(0.0f,dot(norm,intrudelight));\n "//漫反射光照因子
        "vec3 diffuse = diffuseStrength*lightcolor;\n "//漫反射光照（在观察空间里计算）

        "vec3 reflectlight = normalize(reflect(-lightDir,norm));\n "//反射光
//pow(x,y):x的y次方；reflect(L,N):return L-2*dot(L,N)*N; 32是镜面反射强度
        "vec3 viewDir = normalize(-vertexPos);\n "
        "float specularStrength = pow(max(0.0f,dot(reflectlight,viewDir)),32);\n "//镜面反射因子
        "vec3 specular = 10*specularStrength*lightcolor;\n "//镜面反射

//mix(x,y,a):return x*(1-a)+y*a
        "vec4 ColorAddTexture = mix(texture(Tex1,Texcoord),texture(Tex2,Texcoord),0.2);\n "//纹理
        "vec4 result = vec4(ambient + diffuse + specular,1.0);\n "
        "color = result*ColorAddTexture; \n"
    "}\n"
};

const static char vertexShaderforSphere[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec3 vertex;\n "
    "out vec3 outcolor;\n "
    "uniform mat4 model;\n "//变换矩阵;
    "uniform mat4 view;\n "
    "uniform mat4 projection;\n "
    "void main(){\n "
        "gl_Position = projection*view*model*vec4(vertex,1.0f);\n "
    "}\n"
};

const static char fragmentShaderforSphere[] = {
    "#version 450 core\n"
    "out vec4 color;\n "
    "uniform vec3 lightcolor;\n "
    "void main(){\n"
        "color = vec4(lightcolor,1.0f); \n"
    "}\n"
};

const static char vertexShaderforUntexture[] = {
    "#version 450 core\n"
    "layout(location = 0) in vec3 vertex;\n "
    "layout(location = 1) in vec3 incolor;\n "
    "layout(location = 2) in vec3 normal;\n "
    "out vec3 outcolor;\n "
    "out vec3 vertexPos;\n "//顶点在世界坐标中的位置向量
    "out vec3 Normal;\n "//未处理的法向量
    "uniform mat4 model;\n "//变换矩阵;
    "uniform mat4 view;\n "
    "uniform mat4 projection;\n "
    "void main(){\n "
        "gl_Position = projection*view*model*vec4(vertex,1.0f);\n "
        "vertexPos = vec3(model*vec4(vertex,1.0f));\n "
        "outcolor = incolor;\n "
        "Normal = normal;\n "
    "}\n"
};

const static char fragmentShaderforUntexture[] = {
    "#version 450 core\n"
    "in vec3 outcolor;\n "
    "in vec3 vertexPos;\n "//顶点在世界坐标中的位置向量
    "in vec3 Normal;\n "//未处理的法向量
    "out vec4 color;\n "
    "uniform vec3 lightcolor;\n "//灯光颜色
    "uniform vec3 lightPos;\n "//灯光位置
    "uniform vec3 viewPos;\n "//观察者位置
    "uniform mat3 normalModelMat;\n "//正规矩阵
    "uniform mat3 normalViewMat;\n "
    "void main(){\n"
        "float ambientStrength = 0.1f;\n "//环境因子
        "vec3 ambient = ambientStrength*lightcolor;\n "//环境光照

        "vec3 intrudelight = normalize(lightPos - vertexPos);\n "//入射光（方向向量）
        "vec3 norm = normalize(normalModelMat*Normal);\n "
        "float diffuseStrength = max(0.0f,dot(norm,intrudelight));\n "//漫反射光照因子
        "vec3 diffuse = diffuseStrength*lightcolor;\n "//漫反射光照（在世界空间里计算）

        "vec3 reflectlight = normalize(reflect(-lightPos,norm));\n "//反射光
//pow(x,y):x的y次方；reflect(L,N):return L-2*dot(L,N)*N; 32是镜面反射强度
        "vec3 viewDir = normalize(viewPos - vertexPos);\n "
        "float specularStrength = pow(max(0.0f,dot(reflectlight,viewDir)),32);\n "//镜面反射因子
        "vec3 specular = 10*specularStrength*lightcolor;\n "//镜面反射

        "vec4 result = vec4(ambient + diffuse + specular,1.0);\n "
        "color = result*vec4(outcolor,1.0f); \n"
    "}\n"
};

#endif // SHADER_H