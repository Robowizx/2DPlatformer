#define STB_IMAGE_IMPLEMENTATION

#include <Engine/WindowGen.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Shader.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Animation.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 Window gameWindow;
 Mesh *object = new Mesh();
 Shader *program = new Shader();
 GLfloat deltaTime = 0.0f;
 GLfloat lastTime = 0.0f;
 Animation movement;
 Texture crackedsoil;
 char texfile[] = "Textures/hero/hero.png"; 

int main(){
    gameWindow = Window(1280,720);
    GLfloat vertices[] = {
        0.0f,0.0f,  0.6667f,0.8125f,
        0.0f,720.0f,   0.6667f,0.75f,
        1280.0f,0.0f,   1.0f,0.8125f,
        1280.0f,720.0f,    1.0f,0.75f
    };

    float translation = 512.0f, scaling = 0.2f;
    bool leftKey = false;

    if(gameWindow.initialise()==1){
        return 1;
    }
    object->CreateMesh(vertices,16,4);
    object->bindVAO();
    program->CreateFromFiles("Shaders/vertex.glsl","Shaders/fragment.glsl");
    glBindVertexArray(0);

    crackedsoil = Texture(texfile);
    crackedsoil.LoadTexture();

    glm::mat4 projection = glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.0f,-0.0000001f);

    program->UseShader();
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));

    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;
        //Get keyboard inputs
        glfwPollEvents();
        leftKey = movement.keyControl(gameWindow.getsKeys(), leftKey);
    
        if(leftKey){
            translation = 512.0f + 172.0f;
            scaling = -0.2f;
        }
        
        else{
            translation = 512.0f;
            scaling = 0.2f;
        }

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model(1.0f);
        model = glm::translate(model,glm::vec3(translation,232.0f,0.0f));
        model = glm::scale(model,glm::vec3(scaling,0.356f,1.0f));        

        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));

        program->UseShader();

        crackedsoil.UseTexture();

        object->RenderMesh(GL_TRIANGLE_STRIP);

        gameWindow.swapBuffers();
    }
     glUseProgram(0);
     
    return 0;
}
