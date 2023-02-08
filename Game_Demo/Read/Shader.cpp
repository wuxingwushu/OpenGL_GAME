#include "Shader.h"
#include <fstream>
#include <sstream>



#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vertexFile;//创建读档引用
	std::ifstream fragmentFile;
	std::stringstream verteSStream;//创建内存地址引用
	std::stringstream fragmentSStream;

	vertexFile.open(vertexPath);//读档
	fragmentFile.open(fragmentPath);

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//判断文档是否有逻辑错误或文档损坏
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open())//判断是否读档成功
		{
			throw std::exception("open file error");
		}
		verteSStream << vertexFile.rdbuf();//将引用读入内存中
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = verteSStream.str();//将内存中的文档转换成字符串
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();//将字符串转换成CPU可以读取的格式
		fragmentSource = fragmentString.c_str();

		unsigned int vertex, fragment; //创建两个GPU地址
		glewInit();                                     //0x00000000 处(位于 mc_c++.exe 中)引发的异常: 0xC0000005: 执行位置 0x00000000 时发生访问冲突。  的时候就用它
		vertex = glCreateShader(GL_VERTEX_SHADER);//创建着色器
		glShaderSource(vertex, 1, &vertexSource, NULL);//将vertexSource写入vertex，只有一个字符串
		glCompileShader(vertex);//将它二进制化
		checkCompileErrors(vertex,"VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);//创建着色器
		glShaderSource(fragment, 1, &fragmentSource, NULL);//将fragmentSource写入fragment，只有一个字符串
		glCompileShader(fragment);//将它二进制化
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();//创建统一的ID
		glAttachShader(ID, vertex);//放入一个ID
		glAttachShader(ID, fragment);//放入一个ID
		glLinkProgram(ID);//融合ID
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);//删除vertex，因为融入ID中了，它就不用了。
		glDeleteShader(fragment);//删除fragment，因为融入ID中了，它就不用了。
	}
	catch (const std::exception& cuowu)
	{
		printf(cuowu.what());//如果出错打印错误
	}
}

void Shader::use()
{
	glUseProgram(ID);//获取ID
}

void Shader::SetUniform3f(const char* paramNameString, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(ID, paramNameString), param.x, param.y, param.z);
}

void Shader::SetUniform1f(const char* paramNameString, float param)
{
	glUniform1f(glGetUniformLocation(ID, paramNameString), param);
}

void Shader::SetUniform1i(const char* paramNameString, int slot)
{
	glUniform1i(glGetUniformLocation(ID, paramNameString), slot);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type)
{
	int success;//储存是否创建成功
	char infoLog[512];//储存错误报告的大小

	if (type != "PROGRAM")//对应类型进行判断是否错误
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);//将判断是否错误，储存到success
		if (!success)//判断是否错误
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);//将错误报告储存到infoLog
			std::cout << "Shader compile Error:" << infoLog << std::endl;//打印错误
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);//将判断是否错误，储存到success
		if (!success)//判断是否错误
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);//将错误报告储存到infoLog
			std::cout << "program linking Error:" << infoLog << std::endl;//打印错误
		}
	}
}