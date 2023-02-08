#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#define GLUT_DISABLE_ATEXIT_HACK
//#include <GL/glu.h>

#include <map>
#include <ft2build.h>//显示文字
#include FT_FREETYPE_H

#include "./Read/Shader.h"//着色器

#include "./Read/Material.h"//材质

#include "./Light/LightDirectional.h"//平行光
#include "./Light/LightPoint.h"//点光源
#include "./Light/LightSpot.h"//聚光灯

//#define STB_IMAGE_IMPLEMENTATION //图片读取
//#include "stb_image.h"


#include <glm.hpp>//矩阵计算
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>//打印

#include "./Control/Camera.h"//摄像机

#include "./Generate/PerlinNoise.h"//柏林噪声

#include "./Block/blocks.h"//动态生成地形

#include "./Environment/sky.h"//天空盒子

#include "./Mesh/Model.h"//读取模型
#include "./Mesh/Mesh.h"

#include <vector>//动态数组

#include <thread>//多线程

#pragma comment  (lib,"User32.lib")
#pragma comment  (lib,"Gdi32.lib")
#pragma comment  (lib,"Shell32.lib")






//Bullet3D
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <string>
#include "./SharedMemory/PhysicsClientC_API.h"
#include "Bullet3Common/b3Logging.h"
#include "./BulletCollision/Gimpact/btGImpactShape.h"
#include "./BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "./BulletDynamics/Dynamics/btDynamicsWorld.h"




#pragma region 隐藏
#pragma endregion


struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};
map <GLchar, Character> Characters;
GLuint wenVAO, wenVBO;

bool anjian = 1;

const GLuint SHADOW_WIDTH = 15000, SHADOW_HEIGHT = 14000;//阴影图的尺寸
GLuint depthMapFBO;
GLuint depthMap;
glm::mat4 lightSpaceMatrix;
glm::mat4 lightProjection, lightView;

GLFWwindow* window;
const unsigned int SCR_WIDTH = 1960;
const unsigned int SCR_HEIGHT = 1080;
int blockboxs;


string sFPS, sPOS;

#pragma region xxx

//开启调用柏林噪声
//PerlinNoise* Perlin = new PerlinNoise();







sky* skybox = new sky();

blocks* wyblocks = new blocks();






PerlinNoise* Perlinmian = new PerlinNoise();



//初始化相机       位置： X     Y     Z
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 10000.0f);//perspective（相机的开启角度，屏幕的长宽比，从0.1的距离开始光栅化，到100f光栅化结束）

//初始化光源
LightDirectional lightD(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-20.0f), glm::radians(30.0f), glm::radians(-20.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
LightPoint lightP(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
LightSpot lightS(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(0.8f, 0.8f, 0.8f));

//Shader
Shader* testShader;
Shader* shadowShader;
Shader* debugDepthQuad;
Shader* TXTshader;
Shader* MeshShader;



float last_X, last_Y;
bool firstMousse = true;

#pragma endregion


//按键控制
void processInput(GLFWwindow* window);
//鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//打印帧率
void FPS();
//射线检测
glm::vec3 shexianjianche();
void cheshi(GLdouble* shu_, glm::mat4 shuj);
void tc();




//文字显示
void RenderText(Shader* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

string int_chuli(int shuzi);

//阴影
void renderQuad();
unsigned int planeVAO;





int main(int argc, char* argv[])
{
    camera.Position.y = Perlinmian->noise(0, 0, 0.6) * 1024 + 32;//设置摄像机高度（在地面上32）
    /*  显示显卡学习，CUDA环境
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
 
    int dev;
    for (dev = 0; dev < deviceCount; dev++)
    {
        int driver_version(0), runtime_version(0);
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, dev);
        if (dev == 0)
            if (deviceProp.minor = 9999 && deviceProp.major == 9999)
                printf("\n");
        printf("\nDevice%d:\"%s\"\n", dev, deviceProp.name);
        cudaDriverGetVersion(&driver_version);
        printf("CUDA驱动版本:                                   %d.%d\n", driver_version / 1000, (driver_version % 1000) / 10);
        cudaRuntimeGetVersion(&runtime_version);
        printf("CUDA运行时版本:                                 %d.%d\n", runtime_version / 1000, (runtime_version % 1000) / 10);
        printf("设备计算能力:                                   %d.%d\n", deviceProp.major, deviceProp.minor);
        printf("Total amount of Global Memory:                  %u bytes\n", deviceProp.totalGlobalMem);
        printf("Number of SMs:                                  %d\n", deviceProp.multiProcessorCount);
        printf("Total amount of Constant Memory:                %u bytes\n", deviceProp.totalConstMem);
        printf("Total amount of Shared Memory per block:        %u bytes\n", deviceProp.sharedMemPerBlock);
        printf("Total number of registers available per block:  %d\n", deviceProp.regsPerBlock);
        printf("Warp size:                                      %d\n", deviceProp.warpSize);
        printf("Maximum number of threads per SM:               %d\n", deviceProp.maxThreadsPerMultiProcessor);
        printf("Maximum number of threads per block:            %d\n", deviceProp.maxThreadsPerBlock);
        printf("Maximum size of each dimension of a block:      %d x %d x %d\n", deviceProp.maxThreadsDim[0],
            deviceProp.maxThreadsDim[1],
            deviceProp.maxThreadsDim[2]);
        printf("Maximum size of each dimension of a grid:       %d x %d x %d\n", deviceProp.maxGridSize[0], deviceProp.maxGridSize[1], deviceProp.maxGridSize[2]);
        printf("Maximum memory pitch:                           %u bytes\n", deviceProp.memPitch);
        printf("Texture alignmemt:                              %u bytes\n", deviceProp.texturePitchAlignment);
        printf("Clock rate:                                     %.2f GHz\n", deviceProp.clockRate * 1e-6f);
        printf("Memory Clock rate:                              %.0f MHz\n", deviceProp.memoryClockRate * 1e-3f);
        printf("Memory Bus Width:                               %d-bit\n", deviceProp.memoryBusWidth);
    }
 
    return 0;
    */

    string exePath = argv[0];//exe程序路径
#pragma region window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "wu_xing_wu_shu", NULL, NULL);//创建窗口，（大小，窗口的名字，……）
    if (window == NULL)
    {
        printf("窗口创建出现错误");
        glfwTerminate();//结束工作
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标
    glfwSetCursorPosCallback(window, mouse_callback);//获取鼠标移动

    glewExperimental = true;
    if (glfwInit() == 0) //GLFW_FALSE 
    {
        //printf("初始化错误");
        glfwTerminate();//结束工作
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);//设置显示窗口的大小
    glEnable(GL_DEPTH_TEST);//开启 Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)。
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//框线模式
    glEnable(GL_CULL_FACE);//开启剔除
    glCullFace(GL_BACK);//剔除面的背面,//GL_FRONT(正面)


#pragma endregion

#pragma region Shader
    //读取着色器
    testShader = new Shader("./Shader/vertexSource.vert", "./Shader/fragmentSource.frag");
    shadowShader = new Shader("./Shader/shadowShader.vert", "./Shader/shadowShader.frag");
    debugDepthQuad = new Shader("./Shader/shiyan2.vert", "./Shader/shiyan2.frag");
    TXTshader = new Shader("./Shader/TEXTshader.vert", "./Shader/TEXTshader.frag");
    MeshShader = new Shader("./Shader/vertexSource_Mesh.vert", "./Shader/fragmentSource_Mesh.frag");



    /*Shader testShader = Shader("vertexSource.txt", "fragmentSource.txt");               //这些是格式问题要注意
    testShader.test();
    Shader* testShader1 = new Shader("vertexSource.txt", "fragmentSource.txt");
    testShader1->test();*/
#pragma endregion

#pragma region 3D->2D
    glm::mat4 model = {};
    glm::mat4 view = {};
    testShader->use();
    glUniformMatrix4fv(glGetUniformLocation(testShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
#pragma endregion

#pragma region VBO VAO FBO
    //glewExperimental = GL_TRUE;
    //glewInit();                                     //0x00000000 处(位于 mc_c++.exe 中)引发的异常: 0xC0000005: 执行位置 0x00000000 时发生访问冲突。  的时候就用它

    string path = argv[0];
    //Model* model_xxx = new Model(path.substr(0, path.find_last_of("\\")) + "\\model\\nanosuit.obj");//backpack     nanosuit      teddy

    //地形初始化
    //printf("%f ~ %f ~ %f\n", camera.Position.x, camera.Position.y, camera.Position.z);
    wyblocks->init(camera.Position.x, camera.Position.y, camera.Position.z);
    blockboxs = wyblocks->blockboxshu;
    skybox->init();
    skybox->projection = projection;
    




#pragma region 立方体

    /*
    unsigned int VAO;//创建一个ID
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;//创建一个ID
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //glVertexAttribPointer(几号通道，几个数据为一组，指定数据的类型，是否希望数据被标准化(Normalize)，步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔，位置数据在缓冲中起始位置的偏移量(Offset))

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //点向量
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(3);

    //初始化材质
    Material* myMaterial = new Material(testShader,LoadImageToGPU("./准心.png", GL_RGBA, GL_RGBA, 0),LoadImageToGPU("./container2_specular.png", GL_RGBA, GL_RGBA, 1),glm::vec3(1.0f, 1.0f, 1.0f),64.0);

    // 设置 材质 -> 贴图
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, myMaterial->specular);
    // 设置 材质 -> 材质球
    myMaterial->shader->SetUniform1i("material.diffuse", 0);
    myMaterial->shader->SetUniform1i("material.specular", 1);
    myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
    myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);
    */
#pragma endregion

#pragma endregion

#pragma region 阴影的参数

    
    
    glGenFramebuffers(1, &depthMapFBO);

    
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    
    
    float near_plane = 1.0f, far_plane = 600.0f;
    lightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, near_plane, far_plane);//变量：前四个是阴影在玩家的位置基础上的范围边界-x,+x,-y,+y,后两个就是最进和最远的摄像机可视范围。

#pragma endregion


#pragma region 文字
    TXTshader->use();
    glm::mat4 projection1 = glm::ortho(0.0f, static_cast<GLfloat>(1960), 0.0f, static_cast<GLfloat>(1080));
    glUniformMatrix4fv(glGetUniformLocation(TXTshader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection1));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "./txt/Arial.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &wenVAO);
    glGenBuffers(1, &wenVBO);
    glBindVertexArray(wenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

#pragma endregion
    
    
    std::thread trtc(tc);
    trtc.detach();
    std::cout << "初始化耗时  :   " << glfwGetTime() << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        double sjianxx = glfwGetTime();
        processInput(window);//判读输入的按键，并做出对应的事件

        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//背景颜色（清屏色）(0.2f, 0.3f, 0.3f, 1.0f)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一帧画面，和 Z缓冲(Z-buffer)，也被称为深度缓冲(Depth Buffer)。


        
        glm::vec3 pianyi = glm::vec3(150 * camera.Forward.x, 0, 150 * camera.Forward.z) + camera.Position;//计算阴影生成的中心位置
        lightView = glm::lookAt(pianyi + lightD.angles * 300.0f, pianyi, glm::vec3(0.0, 1.0, 0.0));//告诉他，光从那个位置，物体的位置，光的向量。
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        shadowShader->use();
        shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        wyblocks->cameraForward = camera.Forward;
        
        wyblocks->blockPOS();
        wyblocks->view = camera.GetViewMatrix();
        

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);//剔除面的背面,//GL_FRONT(正面)

        //生成方块阴影
        //double sjian = glfwGetTime();

        wyblocks->shnegchen_block(shadowShader);

        //std::cout << "显示耗时  :   " << glfwGetTime() - sjian << std::endl;

        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 1960, 1080);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*查看阴影贴图
        debugDepthQuad->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderQuad();*/

        /**/
        testShader->use();
        glm::mat4 view = camera.GetViewMatrix();
        testShader->setMat4("projection", projection);
        testShader->setMat4("view", view);
        // set light uniforms
        testShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);//导入阴影贴图




        //生成方块
        wyblocks->shnegchen_block(testShader);
        /*
        MeshShader->use();
        MeshShader->setMat4("projMat", projection);
        MeshShader->setMat4("viewMat", view);
        MeshShader->setMat4("modelMat", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));*/
        MeshShader->use();
        //model_xxx->Draw(MeshShader);



        RenderText(TXTshader, ".", 976, 543, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        FPS();
        RenderText(TXTshader, sFPS, 20.0f, 1020.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        //RenderText(TXTshader, sPOS, 20.0f, 990.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));


        //天空盒
        skybox->skyboxxian(camera.GetViewMatrix());


        glfwSwapBuffers(window);//交换显示的缓冲区
        glfwPollEvents();//检测你按下了那些按键
        camera.UpdateCameraPos();//摄像头的移动
        //std::cout << "显示耗时  :   " << glfwGetTime() - sjianxx << std::endl;
    }
    glfwTerminate();//结束工作
    return 0;
}

void tc() {
    //区块移动
    while (!glfwWindowShouldClose(window))
    {
        //double  sjianxxx = glfwGetTime();
        wyblocks->camerapos = camera.Position;
        if(wyblocks->shengchen_kai == false)wyblocks->blockPOSshengchen();
        wyblocks->Threadblock_shnegchen_block_kai();
        //std::cout << "生成耗时  :   " << glfwGetTime() - sjianxxx << std::endl;
    }
}




//按键控制
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
        

    static double _lastTime;
    double deltaTime = glfwGetTime() - _lastTime;
    _lastTime = glfwGetTime();
    //printf("%f", deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.speedZ = -20.0f * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.speedZ = 20.0f * deltaTime;
    }
    else
    {
        camera.speedZ = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.speedX = -20.0f * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.speedX = 20.0f * deltaTime;
    }
    else
    {
        camera.speedX = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.speedY = -20.0f * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.speedY = 20.0f * deltaTime;
    }
    else
    {
        camera.speedY = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.speedY = 200.0f * deltaTime;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && anjian)
    {
        anjian = 0;
        glm::vec3 posz = shexianjianche();
        if (posz.x < 0)posz.x -= blockboxs;
        if (posz.y < 0)posz.y -= blockboxs;
        if (posz.z < 0)posz.z -= blockboxs;
        printf("%f  %f  %f\n", posz.x, posz.y, posz.z);
        float bx, by, bz;
        if (camera.Forward.x > 0) bx = posz.x + 0.05;
        else bx = posz.x - 0.05;
        if (camera.Forward.y > 0) by = posz.y + 0.05;
        else by = posz.y - 0.05;
        if (camera.Forward.z > 0) bz = posz.z + 0.05;
        else bz = posz.z - 0.05;
        int blockshui = (int(bx) / blockboxs + wyblocks->pos_pianyi) - int(camera.Position.x) / blockboxs + (int(bz) / blockboxs + wyblocks->pos_pianyi - int(camera.Position.z) / blockboxs) * wyblocks->bianshu + ((int(by) / blockboxs) + wyblocks->pos_pianyi - int(camera.Position.y) / blockboxs) * wyblocks->bianshu_2;
        if (wyblocks->blockshuliang > blockshui)
        {
            wyblocks->POSjiaozhun(posz.x, posz.y, posz.z, blockshui);
            wyblocks->shuaxinblock(blockshui);
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && anjian)
    {
        anjian = 0;
        glm::vec3 posz = shexianjianche();
        if (posz.x < 0)posz.x -= blockboxs;
        if (posz.y < 0)posz.y -= blockboxs;
        if (posz.z < 0)posz.z -= blockboxs;
        float bx, by, bz;
        if (camera.Forward.x < 0) bx = posz.x + 0.05;
        else bx = posz.x - 0.05;
        if (camera.Forward.y < 0) by = posz.y + 0.05;
        else by = posz.y - 0.05;
        if (camera.Forward.z < 0) bz = posz.z + 0.05;
        else bz = posz.z - 0.05;
        int blockshui = (int(bx) / blockboxs + wyblocks->pos_pianyi) - int(camera.Position.x) / blockboxs + (int(bz) / blockboxs + wyblocks->pos_pianyi - int(camera.Position.z) / blockboxs) * wyblocks->bianshu + ((int(by) / blockboxs) + wyblocks->pos_pianyi - int(camera.Position.y) / blockboxs) * wyblocks->bianshu_2;
        if (wyblocks->blockshuliang > blockshui)
        {
            wyblocks->POSjiaozhuny(posz.x, posz.y, posz.z, blockshui);
            wyblocks->shuaxinblock(blockshui);
        }
    }
    static int ticks = -1;
    ticks++;
    if (ticks == 10)
    {
        anjian = 1;
        ticks = 0;
    }

}
//鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMousse == true)
    {
        last_X = xpos;
        last_Y = ypos;
        firstMousse = false;
    }
    float xoffset = xpos - last_X;
    float yoffset = ypos - last_Y;

    last_X = xpos;
    last_Y = ypos;

    camera.ProcessMouseMovent(yoffset, xoffset);
}
/*
//加载贴图
unsigned int LoadImageToGPU(const char* filename, GLint internalFormart, GLenum format, int textureSlot)
{
    unsigned int TexBuffer; //创建一个ID
    glGenTextures(1, &TexBuffer);//告诉他，这是材质ID，有多少个
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, TexBuffer);//这个材质，是什么类型的

    int width, height, nrChannel;//创建保存，长，宽，颜色通道的个数
    //stbi_set_flip_vertically_on_load(true);//贴图反转
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannel, false);//获取图片的，长，宽，颜色通道的个数
    if (data)//判断数据有无问题
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormart, width, height, false, format, GL_UNSIGNED_BYTE, data);
        //第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
        //第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
        //第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
        //第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
        //下个参数应该总是被设为0（历史遗留的问题）。
        //第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
        //最后一个参数是真正的图像数据。
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("load image failed.");//出差，打印
    }
    stbi_image_free(data);//删除数据，清除内存,保持良好习惯
    return TexBuffer;
}*/

//打印帧率
void FPS()
{
    static int ticks = -1;
    ticks++;
    static double _lastTime = glfwGetTime();
    if (ticks == 40)
    {
        double deltaTime = glfwGetTime() - _lastTime;
        _lastTime = glfwGetTime();

        //std::cout << "帧数" << ticks / deltaTime << std::endl;
        sFPS = "FPS: ";
        sFPS += int_chuli(int(ticks / deltaTime));
        ticks = 0;
    }
}

//射线检测
glm::vec3 shexianjianche()
{
    GLdouble modelview[16], projection1[16];//模型投影矩阵
    GLint viewport[4];//视口
    GLfloat ScreenZ;//深度值
    GLdouble objx, objy, objz;//获得的世界坐标值
    GLdouble nx = 980.0f, ny = 540.0f;

    //glGetDoublev(GL_PROJECTION_MATRIX, projection);//获得投影矩阵
    //glGetDoublev(GL_MODELVIEW_MATRIX, modelview);//获得模型矩阵

    cheshi(projection1, projection);
    cheshi(modelview, glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));


    glGetIntegerv(GL_VIEWPORT, viewport);	//获得视口
    glReadPixels(980, viewport[3] - 540, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &ScreenZ); //获得屏幕像素对应的世界坐标深度值
    gluUnProject(nx, viewport[3] - ny, ScreenZ, modelview, projection1, viewport, &objx, &objy, &objz);//获得屏幕坐标对应的模型坐标

    float x, y, z;

    x = camera.Position.x + camera.Forward.x * -objz;
    y = camera.Position.y + camera.Forward.y * -objz;
    z = camera.Position.z + camera.Forward.z * -objz;


    sPOS.clear();
    sPOS += " X:";
    sPOS += int_chuli((int)x);
    sPOS += " Y:";
    sPOS += int_chuli((int)y);
    sPOS += " Z:";
    sPOS += int_chuli((int)z);

    return glm::vec3(x, y, z);
}
void cheshi(GLdouble* shu_, glm::mat4 shuj)
{
    shu_[0] = shuj[0].x;
    shu_[1] = shuj[0].y;
    shu_[2] = shuj[0].z;
    shu_[3] = shuj[0].w;
    shu_[4] = shuj[1].x;
    shu_[5] = shuj[1].y;
    shu_[6] = shuj[1].z;
    shu_[7] = shuj[1].w;
    shu_[8] = shuj[2].x;
    shu_[9] = shuj[2].y;
    shu_[10] = shuj[2].z;
    shu_[11] = shuj[2].w;
    shu_[12] = shuj[3].x;
    shu_[13] = shuj[3].y;
    shu_[14] = shuj[3].z;
    shu_[15] = shuj[3].w;
}
//文字显示
void RenderText(Shader* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    shader->use();
    shader->SetUniform3f("textColor", color);
    //glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(wenVAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, wenVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

string int_chuli(int shuzi)
{
    char shu[100]{};
    int i = -1;
    while (shuzi != 0)
    {
        i++;
        shu[i] = (shuzi % 10 + 48);
        shuzi = (shuzi - shuzi % 10) / 10;
    }
    string CHARS;
    for (int j = i; j >= 0; j--)
    {
        CHARS.push_back(shu[j]);
    }
    return CHARS;
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {

            // positions        // texture Coords
             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
              1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

              /* // positions        // texture Coords
              -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
              -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
               1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
               1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
               */
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}