#pragma once

#define IDLE "idle"
#define RUN "run"
#define JUMP "jump"
#define ATTACK_1 "attack1"
#define ATTACK_2 "attack2"

#include <vector>
#include <string>
#include <cstring>
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
        Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, int dir);
        void render(GLfloat deltatime);
    private:
        void LRBT();
        void meshInit();
        GLfloat posx, posy, gravity,velX,initalVY,finalVY,timea,change,L,R,B,T;
        int direction,frame;
        std::string state;
        bool debug,*keys;
        Texture tex;
        Json::Value animation;
        Json::Value frames; 
        Json::Value order;
        GLuint modeloc;
        float vertices[8] = {
                              0.0f,0.0f,    
                              0.0f,256.0f,   
                              256.0f,0.0f,   
                              256.0f,256.0f 
                           };        
        float bounds[4] = {0.0f,1280.0f,0.0f,720.0f};                                      
        std::vector<Mesh> Meshlist;
};