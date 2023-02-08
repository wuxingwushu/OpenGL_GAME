#include "sky.h"

#define STB_IMAGE_IMPLEMENTATION //图片读取
#include "../Read/stb_image.h"

//天空盒
float skyVertices[] = {
    //前面      
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,

    //后面
    -1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f,

    //左面
     1.0f, -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 0.0f,

     //右面
    -1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, 0.0f,

    //上面
    -1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f, -1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,

    //底面
    -1.0f, -1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
};

sky::sky()
{
}

void sky::init()
{
    skybox();
    //loadCubemap();
    skyboxShader = new Shader("./Shader/skybox.vert", "./Shader/skybox.frag");
}

void sky::skybox()
{
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), &skyVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
}

unsigned int sky::loadCubemap()
{
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return 0;
}

void sky::skyboxxian(glm::mat4 model)
{
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();
    glm::mat4 view = glm::mat4(glm::mat3(model)); // remove translation from the view matrix
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    skyboxShader->SetUniform3f("Color", glm::vec3(0.0f,0.7f, 0.7f));
    skyboxShader->SetUniform3f("Color1", glm::vec3(0.7f, 0.0f, 0.0f));
    glBindVertexArray(skyboxVAO);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
