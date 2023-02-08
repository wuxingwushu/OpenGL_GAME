#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include "block.h"//动态生成地形

#include "../Read/Shader.h"//着色器

#include "../Light/LightDirectional.h"//平行光
#include "../Light/LightPoint.h"//点光源
#include "../Light/LightSpot.h"//聚光灯

#include "../Control/Camera.h"//摄像机

#include <glm.hpp>    //矩阵计算
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <thread>//多线程
#include <mutex>
#include <atomic>

class blocks
{
public:
	blocks();

	unsigned int threadshu = (std::thread::hardware_concurrency()/2)-2;



	//初始化光源
	LightDirectional lightD = LightDirectional(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-20.0f), glm::radians(30.0f), glm::radians(-20.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
	LightPoint lightP = LightPoint(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
	LightSpot lightS = LightSpot(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(0.8f, 0.8f, 0.8f));

	int Blockposx, Blockposy, Blockposz;//

	const unsigned int bianshu = 18;//正方形的区块边数量
	const unsigned int bianshu_2 = bianshu * bianshu;
	const unsigned int blockshuliang = bianshu * bianshu * bianshu;
	int pos_pianyi = bianshu/2;//计算区块的偏移量
	int shiye_pianyi;
	int shiye_pianyi_ban;

	bool*** Blockkong;//定义三维动态数组
	bool*** shnegchen_Blockkong;//定义三维动态数组
	unsigned int* VAOBOX = new unsigned int[blockshuliang + 1];//定义动态VAO
	unsigned int* VBOBOX = new unsigned int[blockshuliang + 1];//定义动态VBO
	block** blockss = new block * [blockshuliang + 1];//定义动态区块数
	bool* kai = new bool[blockshuliang];
	int blockboxshu;

	bool shengchen_kai = false;

	
	glm::vec3 camerapos;//
	glm::vec3 cameraForward;
	glm::mat4 view;

	

	
	//初始话
	void init(double camerax, double cameray, double cameraz);
	//确认位置z
	void POSjiaozhun(float x, float y, float z, int blockshui);
	//确认位置y
	void POSjiaozhuny(float x, float y, float z, int blockshui);
	//区块移动
	void blockPOS();
	void blockPOSshengchen();

	//区块附近
	void gengxinblock(int x, int y, int z, int blockshui, int px, int py, int pz);
	//区块附近y
	void gengxinblocky(int x, int y, int z, int blockshui, int px, int py, int pz);
	//刷新区块
	void shuaxinblock(int blockshui);

	//显示地形
	void shnegchen_block(Shader* ShaderB);
	//多线程调度初始话
	void Threadblockinit(int a, int b);
	void Threadblockxinit(int x, int y, int z);
	void Threadblock_shnegchen_block(int a, int b);
	void Threadblock_shnegchen_block_kai();
};

