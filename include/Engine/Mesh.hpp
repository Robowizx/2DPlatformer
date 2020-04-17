#pragma once

#include<iostream>
#include <GLEW/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMesh(GLfloat *vertices, unsigned int size, unsigned int numOfvertices);
	void RenderMesh(GLenum primitive);
	void bindVAO();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount,vertexCount;
};

