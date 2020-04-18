#define STB_IMAGE_IMPLEMENTATION

#include <Engine/WindowGen.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Texture.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Mesh *object = new Mesh();
 Shader *program = new Shader();
 GLfloat deltaTime = 0.0f;
 GLfloat lastTime = 0.0f;
 Texture crackedsoil;
 char texfile[] = "Textures/crackedsoil.jpeg"; 

int main(){
    gameWindow = Window(800,600);
    GLfloat vertices[] = {
        400.0f,600.0f, 0.5f, 1.0f,
        0.0f,0.0f,     0.0f, 0.0f,
        800.0f,0.0f,   1.0f, 0.0f
    };

    if(gameWindow.initialise()==1){
        return 1;
    }
    object->CreateMesh(vertices,12,3);
    object->bindVAO();
    program->CreateFromFiles("Shaders/vertex.glsl","Shaders/fragment.glsl");
    glBindVertexArray(0);

    crackedsoil = Texture(texfile);
    crackedsoil.LoadTexture();

    glm::mat4 model(1.0f);
    glm::mat4 projection = glm::ortho(0.0f,800.0f,0.0f,600.0f,0.0f,-0.0000001f);

    program->UseShader();
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));

    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;

        glfwPollEvents();

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        crackedsoil.UseTexture();

        object->RenderMesh(GL_TRIANGLES);

        gameWindow.swapBuffers();
    }
     glUseProgram(0);
     
    return 0;
}
