#ifndef SHADER_H
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
    "struct DirectionalLight{\n"
        "vec3 light;\n " //灯光颜色
        "vec3 direction;\n "//灯光颜色
        "float ambient;\n "
        "float diffuse;\n"
        "float specular;\n "
    "};\n"
    "uniform DirectionalLight directionallight;\n "
    "struct Material{\n"//物体材质
        "sampler2D diffuse;\n "//漫反射材质(漫反射贴图)
        "sampler2D specular;\n "//镜面反射材质(镜面反射贴图)
        "float shineness;\n "//光照散射系数（越低越散）
    "};\n "
    "uniform Material material;\n "//物体材质一致性变量
    "void main(){\n"
        "vec3 Direction = directionallight.direction;\n "//顶点到光源的方向

        "vec3 ambient = directionallight.ambient*directionallight.light*vec3(texture(material.diffuse,Texcoord));\n "//环境光照

        "vec3 intrudelight = normalize(Direction);\n "//入射光（方向向量）
        "vec3 norm = normalize(Normal);\n "
        "float diffuseStrength = max(0.0f,dot(norm,intrudelight));\n "//漫反射光照因子
        "vec3 diffuse = diffuseStrength*directionallight.diffuse*directionallight.light*vec3(texture(material.diffuse,Texcoord));\n "//漫反射光照（在观察空间里计算）

        "vec3 reflectlight = normalize(reflect(-Direction,norm));\n "//反射光
//pow(x,y):x的y次方；reflect(L,N):return L-2*dot(L,N)*N; 32是镜面反射强度
        "vec3 viewDir = normalize(-vertexPos);\n "
        "float specularStrength = pow(max(0.0f,dot(reflectlight,viewDir)),material.shineness);\n "//镜面反射因子
        "vec3 specular = 10*specularStrength*vec3(texture(material.specular,Texcoord))*directionallight.specular*directionallight.light;\n "//镜面反射

//mix(x,y,a):return x*(1-a)+y*a        
        "vec4 result = vec4(ambient + diffuse + specular,1.0);\n "
        "color = result; \n"
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
        "Normal = normalViewMat*normalModelMat*normal;\n "
    "}\n"
};

const static char fragmentShaderforUntexture[] = {
    "#version 450 core\n"
    "in vec3 outcolor;\n "
    "in vec3 vertexPos;\n "//顶点在世界坐标中的位置向量
    "in vec3 Normal;\n "//未处理的法向量
    "in mat4 transformMat;\n "
    "out vec4 color;\n "    
    "uniform vec3 lightPos;\n "//灯光位置
    "struct Light{\n"
        "vec3 light;\n "//灯光颜色
        "float ambient;\n "
        "float diffuse;\n"
        "float specular;\n "
    "};\n"
    "uniform Light light;\n "//光照对不同光照类型的强度
    "struct Material{\n"//物体材质
        "vec3 ambient;\n"//环境光材质
        "vec3 diffuse;\n "//漫反射材质
        "vec3 specular;\n "//镜面反射材质
        "float shineness;\n "//光照散射系数（越低越散）
    "};\n "
    "uniform Material material;\n "//物体材质一致性变量
    "void main(){\n"
        "vec3 lightDir = vec3(transformMat*vec4(lightPos,1.0f));\n "

        "vec3 ambient = light.ambient*light.light*material.ambient;\n "//环境光照

        "vec3 intrudelight = normalize(lightDir - vertexPos);\n "//入射光（方向向量）
        "vec3 norm = normalize(Normal);\n "
        "float diffuseStrength = max(0.0f,dot(norm,intrudelight));\n "//漫反射光照因子
        "vec3 diffuse = diffuseStrength*material.diffuse*light.diffuse*light.light;\n "//漫反射光照

        "vec3 reflectlight = normalize(reflect(-lightDir,norm));\n "//反射光
//pow(x,y):x的y次方；reflect(L,N):return L-2*dot(L,N)*N; 32是镜面反射强度
        "vec3 viewDir = normalize(-vertexPos);\n "
        "float specularStrength = pow(max(0.0f,dot(reflectlight,viewDir)),material.shineness);\n "//镜面反射因子
        "vec3 specular = 10*specularStrength*material.specular*light.specular*light.light;\n "//镜面反射

        "vec4 result = vec4(ambient + diffuse + specular,1.0);\n "
        "color = result; \n"
    "}\n"
};

#endif // SHADER_H
