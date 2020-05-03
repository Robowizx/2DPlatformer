#pragma once

#define IDLE "idle"
#define RUN "run"
#define ATTACK_1 "attack1"
#define ATTACK_2 "attack2"
#define JUMP "jump"
#define FALL "fall"
#define HURT "hurt"
#define DEATH "death"
#define WIN "win"
#define LOSE "lose"
#define ANIM_SPEED 0.0667f
#define GRAVITY -64.0f
#define R_SPEED 480.0f
#define RJ_SPEED 480.0f
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
        Character(GLfloat x,GLfloat y,char* mfile, char*tfile,char* dmfile,char* dtfile, bool wh, bool dbug, bool* k,bool dir, Shader* prg);
        void render(GLfloat deltatime);
        bool setDirection(bool manual); 
        void setState(std::string st,GLfloat deltatime);
        void setDeath(GLfloat deltatime);
        ~Character();

        GLfloat hurt_speed,L,R,B,T,AL,AR,AB,AT,posx,posy;
        bool direction,hitflag,who;
        int frame;
        std::string state;

    private:
        void LRBT();
        void ALRBT();
        void gforce(GLfloat deltatime);
        void stateUpdate(GLfloat deltatime);
        void setRun(GLfloat deltatime,GLfloat velX);
        void setFall(GLfloat deltatime);
        void setJump(GLfloat deltatime);
        void setAttack(GLfloat deltatime);
        void setHurt(GLfloat deltatime);
        void doDeath(GLfloat deltatime);
        GLfloat initialVY,finalVY,scale,timea,ipos,timef,idir,finalVX,iposx,iposy;
        char* Dmeta;
        int anim_index;
        bool debug,*keys;
        Shader* program;
        glm::mat4 model;
        Json::Value animation, frames, meta, order;
        Texture tex, Dtex;
        std::vector<Mesh*> objects;
        GLfloat bound[4] = {0.0f,1280.0f,157.0f,720.0f};
        GLfloat vertices[8] = {
            0.0f,0.0f,
            0.0f,256.0f,
            256.0f,0.0f,
            256.0f,256.0f
        };
            
};