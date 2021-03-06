#pragma once

#include <iostream>
#include <GLEW/glew.h>
#include <stb_image.h>

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture();
	void UseTexture(GLenum texunit);
	void ClearTexture(GLenum texunit);

	~Texture();

private:
	GLuint textureID;
	GLenum tunit;
	int width, height, bitDepth;

	char* fileLocation;
};

