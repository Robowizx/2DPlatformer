#pragma once

#define IDLE 0
#define RUN 1
#define JUMP 2
#define ATTACK_1 3
#define ATTACK_2 4

#include <vector>
#include <fstream>
#include <json/json.h>
#include <GLEW/glew.h>

#include "Mesh.hpp"
#include "Texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Character
{
    public:
        Character();
        Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, int dir,GLint mloc);
        void render(GLfloat deltatime);
    private:
        GLfloat posx, posy, gravity,velX,initalVY,finalVY,timea,change;
        int state,direction;
        bool debug,*keys;
        Texture tex;
        Json::Value animation;
        Json::Value frames; 
        GLuint modeloc;
        float vertices[16] = {
                             0.0f,0.0f,    
                             0.0f,256.0f,   
                             256.0f,0.0f,   
                             256.0f,256.0f, 
                           };
        std::vector<Mesh> Meshlist;
};