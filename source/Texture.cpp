#include <Engine/Texture.hpp>

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
	tunit = GL_TEXTURE0;
}

Texture::Texture(char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 4);
	if (!texData)
	{
		std::cout<<"failed to load file: "<<fileLocation<<std::endl<<stbi_failure_reason()<<std::endl;
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
}

void Texture::UseTexture(GLenum texunit)
{
	tunit = texunit;
	glActiveTexture(texunit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture(GLenum texunit)
{
	glActiveTexture(texunit);
	glBindTexture(GL_TEXTURE_2D,0);
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}


Texture::~Texture()
{
	ClearTexture(tunit);
}
