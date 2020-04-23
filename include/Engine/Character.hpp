#pragma once

#define IDLE "idle"
#define RUN "run"
#define ATTACK_1 "attack1"
#define ATTACK_2 "attack2"
#define JUMP "jump"
#define FALL "fall"
#define HURT "hurt"
#define DEATH "death"

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <json/json.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>


class Character 
{
    public:
        Character();
        Character(GLfloat x,GLfloat y,char* mfile, char*tfile, bool dbug, bool* k,bool dir, Shader* prg);
        void render(GLfloat deltatime);

    private:
        void LRBT();
        void gforce(GLfloat deltatime);
        bool setDirection(); 
        GLfloat posx,posy,gravity,velX,initialVY,finalVY,finalVX,scale,timea,change,L,R,B,T;
        int frame;
        bool debug,direction,*keys;
        std::string state;
        Shader* program;
        glm::mat4 model;
        Json::Value animation, frames, meta, order;
        Texture tex;
        Mesh object;
        GLfloat bound[4] = {0.0f,1280.0f,0.0f,720.0f};
        GLfloat vertices[8] = {
            0.0f,0.0f,
            0.0f,256.0f,
            256.0f,0.0f,
            256.0f,256.0f
        };
            
};