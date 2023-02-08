#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>//动态数组
using namespace std;

#include <iostream>//打印

#include "../Read/Material.h"//着色器

#include <glm.hpp>    //矩阵计算

class sky
{
public:
    sky();

    void init();

    Shader* skyboxShader;

    glm::mat4 projection;
    unsigned int skyboxVAO, skyboxVBO;

    vector<std::string>faces
    {
        "./skybox/right.jpg",
        "./skybox/left.jpg",
        "./skybox/top.jpg",
        "./skybox/bottom.jpg",
        "./skybox/front.jpg",
        "./skybox/back.jpg",
    };
    unsigned int cubemapTexture;


    //天空盒子VAB
    void skybox();
	//天空盒子，贴图载入
	unsigned int loadCubemap();
    //生成天空盒
    void skyboxxian(glm::mat4 model);
};

