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
#define GRAVITY -64.0f
#define R_SPEED 256.0f
#define RJ_SPEED 256.0f
#define J_SPEED 512.0f

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
#include <cmath>


class Character 
{
    public:
        Character();
        Character(GLfloat x,GLfloat y,char* mfile, char*tfile, bool dbug, bool* k,bool dir, Shader* prg);
        void render(GLfloat deltatime);
        void setState(std::string st,GLfloat deltatime);
        ~Character();

        GLfloat hurt_speed,L,R,B,T,AL,AR,AB,AT;
        bool direction,hitflag;
        std::string state;

    private:
        void LRBT();
        void ALRBT();
        void gforce(GLfloat deltatime);
        bool setDirection(); 
        void stateUpdate(GLfloat deltatime);
        void setRun(GLfloat deltatime,GLfloat velX);
        void setFall(GLfloat deltatime);
        void setJump(GLfloat deltatime);
        void setAttack(GLfloat deltatime);
        void setHurt(GLfloat deltatime);
        GLfloat initialVY,finalVY,scale,timea,ipos,timef,posx,posy,idir,finalVX;
        int frame,anim_index;
        bool debug,*keys;
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