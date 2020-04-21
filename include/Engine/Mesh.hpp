#pragma once

#include<iostream>
#include <GLEW/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int size, unsigned int numOfvertices);
	void LoadUV(GLfloat *UV,unsigned int size);
	void RenderMesh(GLenum primitive);
	void bindVAO();
	GLuint getVAO();
	void ClearUV();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, TBO;
	GLsizei vertexCount;
};

