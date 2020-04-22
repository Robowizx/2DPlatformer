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
 Character hero;
 Shader* program = new Shader();
 Mesh object2;
 char metafile[] = "Textures/hero/hero_calc.json";
 char texfile[] = "Textures/hero/hero.png";
 char vertexloc[] = "Shaders/vertex.glsl";
 char fragmentloc[] = "Shaders/fragment.glsl";
 GLfloat lastime=0.0f,deltatime=0.0f, pos[]={
                                            -0.25f,-0.25f,
                                            -0.25f,0.25f,
                                            0.25f,-0.25f,
                                            0.25f,0.25f
                                          };

int main(){
    if(gameWindow.initialise()==1){
        return 1;
    }
    object2.CreateMesh(pos,8,4);
    hero = Character(512.0f,232.0f,program);
    program->CreateFromFiles(vertexloc,fragmentloc);
    glBindVertexArray(0);
    glm::mat4 projection(1.0f);
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));

    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
        deltatime = now - lastime;
        lastime = now;

        glfwPollEvents();

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        hero.render();

        program->UseShader();
        glm::mat4 model(1.0f);
        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
        object2.RenderMesh(GL_TRIANGLE_STRIP);
        glUseProgram(0);

        gameWindow.swapBuffers();
    }
     
    return 0;
}
