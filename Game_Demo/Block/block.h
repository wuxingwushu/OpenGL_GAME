#pragma once


#include <vector>
using namespace std;


class block
{
public:
	block();
	~block();
	
	struct blockdata
	{
		float* vertex;		//顶点
		//float* vertexColor;	//顶点颜色
		//float* vertexvector;//顶点向量
		long int vertexsizeof;
		long int shu;
	};
	blockdata DATA;

	const int shuliang = 65;

	int DATAA[66][66][66];

	void blockinit(int x, int y, int z);
	void shengchengblock();
	void pohuai(int x,int y,int z, int px, int py, int pz);
	void fangzhi(int x, int y, int z, int px, int py, int pz);
	int jiaozhen(int j);
	void blockgengxin(int x, int y, int z);
	void blockgengxiny(int x, int y, int z,int lei);




	//CUDA
	
};

