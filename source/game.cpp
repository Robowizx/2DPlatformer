#define STB_IMAGE_IMPLEMENTATION
#define DEBUG true
#define RIGHT 1
#define LEFT -1

#include <Engine/WindowGen.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Character.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Character hero;
 Shader* program = new Shader();
 char metafile[] = "Textures/hero/hero_calc.json";
 char texfile[] = "Textures/hero/hero.png";
 char vertexloc[] = "Shaders/vertex.glsl";
 char fragmentloc[] = "Shaders/fragment.glsl";
 GLfloat now,lastime,deltatime;

int main(){
    if(gameWindow.initialise()==1){
        return 1;
    }
    hero = Character(512.0f,232.0f,metafile,texfile,DEBUG,gameWindow.getsKeys(),true,program);
    program->CreateFromFiles(vertexloc,fragmentloc);
    glBindVertexArray(0);

    while(!gameWindow.getShouldClose()){
        glfwPollEvents();

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        hero.render(deltatime);
        gameWindow.swapBuffers();
    }
     
    return 0;
}
