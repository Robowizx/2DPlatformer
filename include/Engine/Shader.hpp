#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include <GLEW/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetDebugLocation();
	GLuint GetSamplerLocation();
	//GLuint GetViewLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel,uniformDebug,uniformSampler;
	//Gluint uniformView;
	std::string ReadFile(const char* fileLocation);
	
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

