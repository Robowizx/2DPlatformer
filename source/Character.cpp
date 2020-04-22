#include <Engine/Character.hpp>

Character::Character()
{
    posx = 0.0f;
    posy = 0.0f;
    program = nullptr;
}

Character::Character(GLfloat x,GLfloat y,Shader* prg)
{
    posx = x;
    posy = y;
    program = prg;

    for(int i =0;i<8;i++){
        if(i%2==0)
            vertices[i] += posx;
        else
            vertices[i] += posy;    
    }

    object.CreateMesh(vertices,8,4);
    object.bindVAO();
}

void Character::render()
{
    program->UseShader();

    glm::mat4 model(1.0f);
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));

    object.RenderMesh(GL_TRIANGLE_STRIP);

    glUseProgram(0);
}