#define STB_IMAGE_IMPLEMENTATION
#define DEBUG false
#define RIGHT 1
#define LEFT -1

#include <Engine/WindowGen.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Character.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Character* hero;
 Shader* program = new Shader();
 char metafile[] = "Textures/hero/hero_calc.json";
 char texfile[] = "Textures/hero/hero.png";
 char vertexloc[] = "Shaders/vertex.glsl";
 char fragmentloc[] = "Shaders/fragment.glsl";
 GLfloat lastime=0.0f,deltatime=0.0f;

int main(){
    gameWindow = Window(1280,720);
    if(gameWindow.initialise()==1){
        return 1;
    }

    hero = new Character(512.0f,232.0f,metafile,texfile,DEBUG, gameWindow.getsKeys(),true,program);
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

        hero->render(deltatime);
  
        gameWindow.swapBuffers();
    }
     
    return 0;
}
