#define STB_IMAGE_IMPLEMENTATION
#define DEBUG false
#define HERO_META "Textures/hero/hero_calc.json"
#define HERO_TEX "Textures/hero/hero.png"
#define ENEMY_META "Textures/enemy/enemy_calc.json"
#define ENEMY_TEX "Textures/enemy/enemy.png"
#define BACK_TEX "Textures/level/Battleground2.png"
#define VERTEX_LOC "Shaders/vertex.glsl"
#define FRAGMENT_LOC "Shaders/fragment.glsl"
#define DEATH_META "Textures/death/death.json"
#define DEATH_TEX "Textures/death/death.png"
#define WIN_TEX "Textures/UI/you_win.png"
#define LOSE_TEX "Textures/UI/you_lose.png"
#define ENTER_TEX "Textures/UI/enter.png"

#include <Engine/WindowGen.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Character.hpp>

#include<cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Character *hero,*enemy;
 int hero_health = 100,enemy_health = 100;
 bool play = true;
 Mesh* background = new Mesh(),*result = new Mesh();
 Texture img,enter,win,lose;
 Shader* program = new Shader();
 GLfloat vertices[] ={
     0.0f,0.0f,
     0.0f,720.0f,
     1280.0f,0.0f,
     1280.0f,720.0f
 };
 GLfloat UV[]={
     0.0f,1.0f,
     0.0f,0.0f,
     1.0f,1.0f,
     1.0f,0.0f
 };
 
 
 GLfloat lastime=0.0f,deltatime=0.0f,delay = 0.0f;

void doBot(GLfloat deltatime){
    
    if(enemy->state == IDLE && enemy->B==157.0f){

        if((enemy->R > hero->L-25.0f && enemy->L < hero->R-25.0f && enemy->direction)||(enemy->L < hero->R+25.0f && enemy->R > hero->L+25.0f && (!enemy->direction)))
        {
            int random = rand()%100+1;
            if(random >= 75)
                enemy->setState(ATTACK_2,deltatime);
            else
                enemy->setState(ATTACK_1,deltatime);        
        }
        else if((enemy->R < hero->L-25.0f && enemy->direction)||(enemy->L > hero->R+25.0f && (!enemy->direction)))
            enemy->setState(RUN,deltatime);
        else
            enemy->setDirection(true);     
    }
    else if(enemy->state == RUN){
        if((enemy->R > hero->L-25.0f && enemy->L < hero->R-25.0f && enemy->direction)||(enemy->L < hero->R+25.0f && enemy->R > hero->L+25.0f && (!enemy->direction)))
        {
            int random = rand()%100+1;
            if(random >= 75)
                enemy->setState(ATTACK_2,deltatime);
            else
                enemy->setState(ATTACK_1,deltatime);        
        }
        else if((enemy->R > hero->L-25.0f && enemy->direction)||(enemy->L < hero->R+25.0f && (!enemy->direction))){
            enemy->setState(IDLE,deltatime);
            enemy->setDirection(true);
        }     
    }
}
void doHits(GLfloat deltatime){
    if(hero->state == HURT || enemy->state == HURT)
        return;
    if(hero->hitflag == true){
        if(hero->direction){
            if(((hero->AR>enemy->L && hero->AR<enemy->R)||(hero->AL>enemy->L && hero->AL<enemy->R))&& enemy->B < hero->AT){
                if(hero->state == ATTACK_1)
                    enemy_health-=10;
                else
                    enemy_health-=20; 

                if(enemy_health < 0){
                    enemy->setDeath(deltatime);
                    hero->state = WIN;
                    hero->frame = 32;
                    result->CreateMesh(vertices,8,4);
                    result->LoadUV(UV,8);
                }
                else
                {
                    enemy->setState(HURT,deltatime);
                    if(enemy->direction)
                        enemy->hurt_speed = R_SPEED;
                    else
                        enemy->hurt_speed = -R_SPEED;
                }            
            }
        }
        else{
            if(((hero->AL<enemy->R && hero->AL>enemy->L)||(hero->AR<enemy->R && hero->AR>enemy->L))&& enemy->B < hero->AT){
                if(hero->state == ATTACK_1)
                    enemy_health-=10;
                else
                    enemy_health-=20;

                if(enemy_health < 0){
                    enemy->setDeath(deltatime);
                    hero->state = WIN;
                    hero->frame = 32;
                    result->CreateMesh(vertices,8,4);
                    result->LoadUV(UV,8);
                }
                else
                {
                    enemy->setState(HURT,deltatime);
                    if(enemy->direction)
                        enemy->hurt_speed = -R_SPEED;
                    else
                        enemy->hurt_speed = R_SPEED;
                }            
            }
        }
    }
    if(enemy->hitflag == true){
       if(enemy->direction){
            if(((enemy->AR>hero->L && enemy->AR<hero->R)||(enemy->AL>hero->L && enemy->AL<hero->R))&& hero->B < enemy->AT){
                if(enemy->state == ATTACK_1)
                    hero_health-=10;
                else
                    hero_health-=20;

                if(hero_health < 0 ){
                    hero->setDeath(deltatime);
                    enemy->state = WIN;
                    enemy->frame = 37;
                    result->CreateMesh(vertices,8,4);
                    result->LoadUV(UV,8);

                }
                else{
                    hero->setState(HURT,deltatime);
                    if(hero->direction)
                        hero->hurt_speed = R_SPEED;
                    else
                        hero->hurt_speed = -R_SPEED;
                }            
            }
        }
        else{
            if(((enemy->AL<hero->R && enemy->AL>hero->L)||(enemy->AR<hero->R && enemy->AR>hero->L))&& hero->B < enemy->AT){
                if(enemy->state == ATTACK_1)
                    hero_health-=10;
                else
                    hero_health-=20;
                if(hero_health < 0){
                    hero->setDeath(deltatime);
                    enemy->state = WIN;
                    enemy->frame = 37;
                    result->CreateMesh(vertices,8,4);
                    result->LoadUV(UV,8);
                }    
                else{
                    hero->setState(HURT,deltatime);
                    if(hero->direction)
                        hero->hurt_speed = -R_SPEED;
                    else
                        hero->hurt_speed = R_SPEED;
                }            
            }
        } 
    }
}
int main(){
    
    gameWindow = Window(1280,720);
    if(gameWindow.initialise()==1){
        return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    background->CreateMesh(vertices,8,4);
    background->LoadUV(UV,8);

    img = Texture(BACK_TEX);
    enter = Texture(ENTER_TEX);
    enter.LoadTexture();
    win = Texture(WIN_TEX);
    lose = Texture(LOSE_TEX);

    program->CreateFromFiles(VERTEX_LOC,FRAGMENT_LOC);
    glBindVertexArray(0);
    
    program->UseShader();
    glm::mat4 projection = glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.0f,-1.0f);
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));
    glUseProgram(0);

restart:
    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        deltatime = now - lastime;
        lastime = now;
        delay+=deltatime;

        glfwPollEvents();
        
        if(gameWindow.getsKeys()[GLFW_KEY_ENTER])
            break;
        
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        if(delay>=0.5f){
            delay = 0.0f;
            play = !play;
        }

        if(play)
        {   
            program->UseShader();
            enter.UseTexture(GL_TEXTURE0);
            glm::mat4 text_model(1.0f);
            text_model = glm::translate(text_model,glm::vec3(320.0f,180.0f,0.0f));
            text_model = glm::scale(text_model,glm::vec3(0.5f,0.5f,1.0f));
            glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(text_model));
            glUniform1i(program->GetDebugLocation(),0);
            background->RenderMesh(GL_TRIANGLE_STRIP);
            glUseProgram(0);
        }

        gameWindow.swapBuffers();   
    }
    
    delay = 0.0f;
    play = true;

    img.LoadTexture();
    win.LoadTexture();
    lose.LoadTexture();

    hero = new Character(0.0f,157.0f,HERO_META,HERO_TEX,DEATH_META,DEATH_TEX,true,DEBUG,gameWindow.getsKeys(),true,program);
    enemy = new Character(1280.0f,157.0f,ENEMY_META,ENEMY_TEX,DEATH_META,DEATH_TEX,false,DEBUG,nullptr,false,program);

    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        deltatime = now - lastime;
        lastime = now;

        glfwPollEvents();

        if(gameWindow.pause)
            continue;       
           
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program->UseShader();
        img.UseTexture(GL_TEXTURE0);
        glm::mat4 background_model(1.0f);
        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(background_model));
        glUniform1i(program->GetDebugLocation(),0);
        background->RenderMesh(GL_TRIANGLE_STRIP);
        glUseProgram(0);

        if(hero->state != WIN && enemy->state != WIN){
            doHits(deltatime);
            doBot(deltatime);
        }
        else
        {   delay+=deltatime;
            if(delay>=0.5f){
                delay=0.0f;
                play = !play;
            }
            if(play){
                program->UseShader();
                if(hero->state == WIN)
                    win.UseTexture(GL_TEXTURE0);
                else
                    lose.UseTexture(GL_TEXTURE0);
                glm::mat4 result_model(1.0f);
                result_model = glm::translate(result_model,glm::vec3(480.0f,360.0f,0.0f));
                result_model = glm::scale(result_model,glm::vec3(0.25f,0.25f,1.0f));
                glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(result_model));
                glUniform1i(program->GetDebugLocation(),0);
                result->RenderMesh(GL_TRIANGLE_STRIP);
                glUseProgram(0);
            } 
            if(gameWindow.getsKeys()[GLFW_KEY_SPACE]){
                result->ClearMesh();
                hero_health = 100;
                enemy_health = 100;
                goto restart;
            }           
        }
            
        if(enemy->state == ATTACK_1 || enemy-> state == ATTACK_2)
        {
            hero->render(deltatime);
            enemy->render(deltatime);
        }
        else{
            enemy->render(deltatime);
            hero->render(deltatime);
        }

        gameWindow.swapBuffers();
    }
     result->ClearMesh();
     background->ClearMesh();
     enter.ClearTexture(GL_TEXTURE0);
     win.ClearTexture(GL_TEXTURE0);
     lose.ClearTexture(GL_TEXTURE0);
     img.ClearTexture(GL_TEXTURE0);
    return 0;
}