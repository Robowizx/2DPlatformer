#pragma once

#define IDLE "idle"
#define RUN "run"
#define JUMP "jump"
#define ATTACK_1 "attack1"
#define ATTACK_2 "attack2"
#define FALL "fall"

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <json/json.h>


#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Character
{
    public:
        Character();
        Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, bool dir, Shader*  prg);
        void render(GLfloat deltatime);
        GLuint getVAO();
        ~Character();

    private:
        void LRBT();
        void meshInit();
        void stateUpdate();
        GLfloat setDirection();
        // void setIdle();
        // void setRun();
        // void setAttack1();
        // void setAttack2();
        // void setJump();
        void gforce(GLfloat deltatime);

        GLfloat posx, posy, gravity,velX,initalVY,finalVY,timea,L,R,B,T,timef;
        int direction,frame;
        std::string state;
        bool debug,*keys;
        Texture tex;
        Json::Value animation;
        Json::Value frames; 
        Json::Value order;
        Json::Value meta;
        Shader* program;
        GLfloat vertices[8] = {
                              0.0f,0.0f,    
                              0.0f,256.0f,   
                              256.0f,0.0f,   
                              256.0f,256.0f 
                           };        
        float bounds[4] = {0.0f,1280.0f,0.0f,720.0f};                                      
        std::vector<Mesh> Meshlist;
};