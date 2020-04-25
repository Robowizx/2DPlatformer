#pragma once

#define IDLE "idle"
#define RUN "run"
#define ATTACK_1 "attack1"
#define ATTACK_2 "attack2"
#define JUMP "jump"
#define FALL "fall"
#define HURT "hurt"
#define DEATH "death"
#define ANIM_SPEED 0.05f
#define GRAVITY -39.2f
#define R_SPEED 480.0f
#define RJ_SPEED 240.0f
#define J_SPEED 612.0f

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
        ~Character();

    private:
        void LRBT();
        void ALRBT();
        void gforce(GLfloat deltatime);
        bool setDirection(); 
        void stateUpdate(GLfloat deltatime);
        void setState(std::string st,GLfloat deltatime);
        void setRun(GLfloat deltatime,GLfloat velX);
        void setFall(GLfloat deltatime);
        void setJump(GLfloat deltatime);
        void setAttack(GLfloat deltatime);
        GLfloat posx,posy,initialVY,finalVY,finalVX,scale,timea,L,R,B,T,AL,AR,AB,AT,ipos,timef;
        int frame,anim_index;
        bool debug,direction,*keys,hitFlag;
        std::string state;
        Shader* program;
        glm::mat4 model;
        Json::Value animation, frames, meta, order;
        Texture tex;
        std::vector<Mesh*> objects;
        GLfloat bound[4] = {0.0f,1280.0f,157.0f,720.0f};
        GLfloat vertices[8] = {
            0.0f,0.0f,
            0.0f,256.0f,
            256.0f,0.0f,
            256.0f,256.0f
        };
            
};