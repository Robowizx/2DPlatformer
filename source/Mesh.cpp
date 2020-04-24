#include <Engine/Mesh.hpp>

 Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	TBO = 0;
    vertexCount = 0;
}



void Mesh::CreateMesh(GLfloat *vertices, unsigned int size, unsigned int numOfVertices){
	vertexCount = numOfVertices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,0, 0);//Attrib pointer for pos
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	//std::cout<<"Mesh created."<<std::endl;
}

GLuint Mesh::getVAO(){
	return VAO;
}

void Mesh::LoadUV(GLfloat *UV,unsigned int size){
	if(VAO){
		glBindVertexArray(VAO);

		glGenBuffers(1,&TBO);
		glBindBuffer(GL_ARRAY_BUFFER,TBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(UV[0])*size,UV,GL_STATIC_DRAW);

		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER,0);

		glBindVertexArray(0);
	}
}

void Mesh::ClearUV(){
	if(TBO !=0){
		glDeleteBuffers(1,&TBO);
		TBO = 0;
	}
}

void Mesh::RenderMesh(GLenum primitive)
{
	if(VAO){
		glBindVertexArray(VAO);
		if(VBO && vertexCount){
    	    glDrawArrays(primitive,0,vertexCount);
    	}
		glBindVertexArray(0);
	}
}

void Mesh::bindVAO(){
	//std::cout<<"VAO = "<<VAO<<std::endl;
	glBindVertexArray(VAO);
}

void Mesh::ClearMesh()
{
	if (TBO != 0)
	{
		glDeleteBuffers(1, &TBO);
		TBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
    vertexCount = 0;
}


Mesh::~Mesh()
{
	ClearMesh();
}
