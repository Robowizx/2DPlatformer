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
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	char* fileLocation;
};

