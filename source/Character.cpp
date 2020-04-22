#include <Engine/Character.hpp>

Character::Character()
{}

Character::Character(GLfloat x,GLfloat y,char* mfile,char* tfile, bool dbug, bool* k, bool dir, Shader* prg)
{
    posx = x;
    posy = y;
    program = prg;
    debug = dbug;
    keys = k;
    gravity = -9.8f;
    initialVY = 0.0f;
    finalVY = 0.0f;
    timea = 0.0f;
    change = 0.0f;
    state = IDLE;
    model = glm::mat4(1.0f);

    for(int i =0;i<8;i++){
        if(i%2==0)
            vertices[i] += posx;
        else
            vertices[i] += posy;    
    }

    tex = Texture(tfile);
    tex.LoadTexture();

    std::ifstream stream(mfile,std::ios::binary);
    Json::Value root;
    stream >> root;
    animation = root["animation"];
    frames = root["frames"];
    meta = root["meta"];
    order = animation[state]["list"];
    frame = order[0].asInt();
    std::cout<<"mirror = "<<frames[frame]["index"].asFloat()<<std::endl;

    LRBT();

    object.CreateMesh(vertices,8,4);
    object.bindVAO();
}

void Character::LRBT()
{
    L = posx + (2.0f*frames[frame]["L"][0].asFloat());
    R = posx + 256.0f - (2.0f*frames[frame]["R"][0].asFloat());
    B = posy + (2.0f*frames[frame]["B"][0].asFloat());
    T = posy + 256.0f - (2.0f*frames[frame]["T"][0].asFloat());
}

void Character::gforce(GLfloat deltatime)
{
    
    LRBT();
    GLfloat diff = B-posy; 
    if(B>bound[2]){
        finalVY = initialVY + (gravity*timea);
        timea += deltatime;
    }
    else{
        timea = 0.0f;
        finalVY = initialVY;
    }
    initialVY = finalVY;

    posy += (finalVY*deltatime);
    if((posy+diff)<bound[2]){

        finalVY = (finalVY*deltatime)-(posy+diff);
        posy -= (posy+diff);
    }
    else{
        finalVY = (finalVY*deltatime);
    }
    
}

GLfloat Character::setDirection(){
    
    
    if((keys[GLFW_KEY_LEFT] && direction) || (keys[GLFW_KEY_RIGHT] && (!direction))){
        std::cout<<"left key = "<<keys[GLFW_KEY_LEFT]<<" right key = "<<keys[GLFW_KEY_RIGHT]<<"direction = "<<direction<<std::endl;
        direction = !direction;
        GLfloat out = frames[frame]["index"].asFloat();
        std::cout<<"index = "<<out<<std::endl;
        return out;
    }
    else{
        return 0.0f;
    }    
}

void Character::render(GLfloat deltatime)
{
    GLfloat imgX = meta["size"]["w"].asFloat();
    GLfloat imgY = meta["size"]["h"].asFloat();
    GLfloat frameX = frames[frame]["x"].asFloat();
    GLfloat frameY = frames[frame]["y"].asFloat();
    GLfloat size = frames[frame]["w"].asFloat();
    GLfloat index=0.0f,scale = 1.0f;
    //std::cout<<"imgX = "<<imgX<<" imgY = "<<imgY<<" frameX = "<<frameX<<" frameY = "<<frameY<<" size = "<<size<<std::endl;
    for(int i = 0;i<8;i++){
       if(i%2==0){

           if(i<=2)
            vertices[i] = frameX/imgX;
           else
            vertices[i] = (frameX+size)/imgX; 
       }
       else{
           if(i==1 || i==5)
            vertices[i] = ((frameY+size)/imgY);
           else
            vertices[i] = (frameY/imgY); 
       }
       //std::cout<<vertices[i]<<" , ";
    }  
    //std::cout<<std::endl;

    object.ClearUV();
    object.LoadUV(vertices,8);

    tex.UseTexture(GL_TEXTURE0);

    program->UseShader();

    //gforce(deltatime);

    gforce(deltatime);
    index = setDirection();
    //std::cout<<"index = "<<index<<std::endl;
    if(index != 0.0f){
        std::cout<<"changing = "<<index<<std::endl;
        scale= -1.0f;
        // if(direction)
        //     posx -= index;
        // else
        //     posx += index;
    }
    model = glm::translate(model,glm::vec3(0.0f,finalVY,0.0f));
    if(index != 0.0f){
        std::cout<<"scale = "<<scale<<std::endl;
    }
    model = glm::scale(model,glm::vec3(scale,1.0f,1.0f));
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniform1i(program->GetDebugLocation(),0);
    glUniform1i(program->GetSamplerLocation(),0);
    
    object.RenderMesh(GL_TRIANGLE_STRIP);
    
    glUseProgram(0);

}

