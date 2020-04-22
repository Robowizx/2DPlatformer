#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


class Character 
{
    public:
        Character();
        Character(GLfloat x,GLfloat y,Shader* prg);
        void render();

    private:
        GLfloat posx,posy;
        Shader* program;
        Mesh object;
        GLfloat vertices[8] = {
            0.0f,0.0f,
            0.0f,256.0f,
            256.0f,0.0f,
            256.0f,256.0f
        };
            
};