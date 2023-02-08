#pragma once

#include <glm.hpp>    //�������
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"//��ɫ��

class Material
{
public:
	Shader * shader;
	unsigned int diffuse;
	unsigned int specular;
	glm::vec3 ambient;
	float shininess;

	Material(Shader* _shader, unsigned int _diffuse, unsigned int _specular, glm::vec3 _ambient, float _shininess);
};

