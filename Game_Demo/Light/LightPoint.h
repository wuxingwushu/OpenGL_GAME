#pragma once

#include <gtx/rotate_vector.hpp>

#include <glm.hpp>    //矩阵计算
//#include <gtc/matrix_transform.hpp>

class LightPoint
{
public:
	LightPoint(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f));
	~LightPoint();

	glm::vec3 position;//光的位置
	glm::vec3 angles;//光的旋转角
	glm::vec3 direction = glm::vec3(0, 0, 1.0f);//光的向量
	glm::vec3 color;//光的颜色

	float constant = 1.0f;
	float linear = 0.1f;
	float quadratic = 0.001f;

	void UpdateDirection();
};

