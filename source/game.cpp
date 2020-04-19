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
 GLfloat gravity=-5.0f,initialv=0.0f,finalv=0.0f,posx=512.0f,posy=232.0f,timea=0.0f,change=0.0f;

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

    float translation = 0.0f, scaling = 1.0f;
    //right = false left = true (state)
    bool leftKey = false,state=false,rightkey=false;

    if(gameWindow.initialise()==1){
        return 1;
    }
    object->CreateMesh(vertices,16,4);
    object->bindVAO();
    program->CreateFromFiles("Shaders/vertex.glsl","Shaders/fragment.glsl");
    glBindVertexArray(0);

    crackedsoil = Texture(texfile);
    crackedsoil.LoadTexture();

    glm::mat4 model(1.0f);
    model = glm::translate(model,glm::vec3(posx,posy,0.0f));
    model = glm::scale(model,glm::vec3(0.2f,0.356f,1.0f));
    glm::mat4 projection = glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.0f,-0.0000001f);

    program->UseShader();
    glUniformMatrix4fv(program->GetProjectionLocation(),1,GL_FALSE,glm::value_ptr(projection));
    crackedsoil.UseTexture();
    while(!gameWindow.getShouldClose()){
        GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;
        if(posy+38>0.0f){
            finalv = initialv + (gravity*timea);
            timea += deltaTime;
            initialv = finalv;
        }
        else
        {
            timea = 0.0f;
            finalv = 0.0f;   
        }
        posy += (finalv*deltaTime);
        if(posy+38<0.0f){
            change=(finalv*deltaTime)-(posy+38);
            posy-=(posy+38); 
        }
        else{
            change = (finalv*deltaTime);
        }
        //std::cout<<posy<<std::endl;

        glfwPollEvents();
        leftKey = movement.keyControl(gameWindow.getsKeys(),GLFW_KEY_LEFT);
        rightkey = movement.keyControl(gameWindow.getsKeys(),GLFW_KEY_RIGHT);
    
        if(leftKey && !state){
            std::cout<<"turning left"<<std::endl;
            translation = 172.0f;
            state = !state;
            scaling = -1.0f;
        }
        else if(rightkey && state){
            std::cout<<"turning right"<<std::endl;
            translation = 172.0f;
            state = !state;
            scaling = -1.0f;
        }

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        model = glm::translate(model,glm::vec3(translation/0.2f,change/0.355f,0.0f));
        model = glm::scale(model,glm::vec3(scaling,1.0f,1.0f));
        translation = 0.0f;
        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
        object->RenderMesh(GL_TRIANGLE_STRIP);
        gameWindow.swapBuffers();
    }
     glUseProgram(0);
     
    return 0;
}
