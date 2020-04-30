#define STB_IMAGE_IMPLEMENTATION
#define DEBUG true

#include <Engine/WindowGen.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Character.hpp>

#include<cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Character *hero,*enemy;
 Mesh* background = new Mesh();
 Texture img;
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
 
 char metafile1[] = "Textures/hero/hero_calc.json";
 char texfile1[] = "Textures/hero/hero.png";
 char metafile2[] = "Textures/enemy/enemy_calc.json";
 char texfile2[] = "Textures/enemy/enemy.png";
 char back[] = "Textures/level/Battleground2.png";
 char vertexloc[] = "Shaders/vertex.glsl";
 char fragmentloc[] = "Shaders/fragment.glsl";
 GLfloat lastime=0.0f,deltatime=0.0f;

void doHits(GLfloat deltatime){
    if(hero->hitflag == true){
        if(hero->direction){
            if((hero->AR>enemy->L && hero->AR<enemy->R)||(hero->AL>enemy->L && hero->AL<enemy->R)){
                enemy->setState(HURT,deltatime);
                if(enemy->direction)
                    enemy->hurt_speed = R_SPEED;
                else
                    enemy->hurt_speed = -R_SPEED;    
            }
        }
        else{
            if((hero->AL<enemy->R && hero->AL>enemy->L)||(hero->AR<enemy->R && hero->AR>enemy->L)){
                enemy->setState(HURT,deltatime);
                if(enemy->direction)
                    enemy->hurt_speed = -R_SPEED;
                else
                    enemy->hurt_speed = R_SPEED;    
            }
        }
    }
    if(enemy->hitflag == true){
       if(enemy->direction){
            if((enemy->AR>hero->L && enemy->AR<hero->R)||(enemy->AL>hero->L && enemy->AL<hero->R)){
                hero->setState(HURT,deltatime);
                if(hero->direction)
                    hero->hurt_speed = R_SPEED;
                else
                    hero->hurt_speed = -R_SPEED;    
            }
        }
        else{
            if((enemy->AL<hero->R && enemy->AL>hero->L)||(enemy->AR<hero->R && enemy->AR>hero->L)){
                hero->setState(HURT,deltatime);
                if(hero->direction)
                    hero->hurt_speed = -R_SPEED;
                else
                    hero->hurt_speed = R_SPEED;    
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
    img = Texture(back);
    img.LoadTexture();
    hero = new Character(512.0f,157.0f,metafile1,texfile1,DEBUG,nullptr,true,program);
    enemy = new Character(0.0f,157.0f,metafile2,texfile2,DEBUG,gameWindow.getsKeys(),true,program);
    program->CreateFromFiles(vertexloc,fragmentloc);
    glBindVertexArray(0);
    program->UseShader();
    glm::mat4 projection = glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.0f,-1.0f);
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));
    glUseProgram(0);

    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        deltatime = now - lastime;
        lastime = now;

        glfwPollEvents();

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program->UseShader();
        img.UseTexture(GL_TEXTURE0);
        glm::mat4 test(1.0f);
        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(test));
        glUniform1i(program->GetDebugLocation(),0);
        background->RenderMesh(GL_TRIANGLE_STRIP);
        glUseProgram(0);

        doHits(deltatime);
        hero->render(deltatime);
        enemy->render(deltatime);
  
        gameWindow.swapBuffers();
    }
     
    return 0;
}