#pragma once

/*#ifndef CAMERA_H//判断是否编译过（防止反复编译），#pragma once和这个一样所以不用了
#define CAMERA_H
//内容
#endif // !CAMERA_H*/

#include <glm.hpp>    //矩阵计算
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	~Camera();

	glm::vec3 Position;//相机位置
	glm::vec3 Forward;//前视
	glm::vec3 Right;//右视
	glm::vec3 Up;//上视
	glm::vec3 WorldUp;//世界位置

	float Pitch;
	float Yaw;

	float speedZ;
	float speedY;
	float speedX;

	glm::mat4 GetViewMatrix();
	void ProcessMouseMovent(float deltaX, float deltaY);
	void UpdateCameraPos();
private:
	void UpdateCameraVectors();
};


