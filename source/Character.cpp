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
    direction = dir;
    gravity = -9.8f;
    velX = 128.0f;
    initialVY = 0.0f;
    finalVY = 0.0f;
    finalVX = 0.0f;
    scale = 1.0f;
    timea = 0.0f;
    change = 0.0f;
    state = IDLE;
    model = glm::mat4(1.0f);

    for(int i=0;i<8;i++){
        if(i%2==0){
           if(!direction && i>2)
            vertices[i]+=(posx-512.0f);
           else
            vertices[i]+=posx; 
        }    
        else
            vertices[i]+=posy;
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
   // std::cout<<"mirror = "<<frames[frame]["index"].asFloat()<<std::endl;

    LRBT();

    object.CreateMesh(vertices,8,4);
    object.bindVAO();
}

void Character::LRBT()
{
    if(direction){
        L = posx + (2.0f*frames[frame]["L"][0].asFloat());
        R = posx + 256.0f - (2.0f*frames[frame]["R"][0].asFloat());
        B = posy + (2.0f*frames[frame]["B"][0].asFloat());
        T = posy + 256.0f - (2.0f*frames[frame]["T"][0].asFloat());
   }
   else{
        R = posx - (2.0f*frames[frame]["L"][0].asFloat());
        L = posx - 256.0f + (2.0f*frames[frame]["R"][0].asFloat());
        B = posy + (2.0f*frames[frame]["B"][0].asFloat());
        T = posy + 256.0f - (2.0f*frames[frame]["T"][0].asFloat());
    }
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
        //std::cout<<"shutting gravity"<<std::endl;
        timea = 0.0f;
        finalVY = initialVY;
    }
    initialVY = finalVY;

    //std::cout<<posy<<" "<<B<<" "<<diff<<std::endl;
    posy += (finalVY*deltatime);
    
    if((posy+diff)<bound[2]){

        finalVY = (finalVY*deltatime)-((posy+diff)-bound[2]);
        posy -= ((posy+diff)-bound[2]);
    }
    else{
        finalVY = (finalVY*deltatime);
    }
    
}

bool Character::setDirection(){
    
    
    if(((keys[GLFW_KEY_LEFT] && direction) || (keys[GLFW_KEY_RIGHT] && (!direction))) && (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]))){
       // std::cout<<"left key = "<<keys[GLFW_KEY_LEFT]<<" right key = "<<keys[GLFW_KEY_RIGHT]<<"direction = "<<direction<<std::endl;
        direction = !direction;
        scale= -1.0f;
        GLfloat index = frames[frame]["index"].asFloat();
        if(!direction){
            finalVX += 2.0f*(posx-index)+256.0f;
            posx += (256.0f-(2.0f*index));
        }
        else{
            finalVX += 2.0f*(posx+index)-256.0f;
            posx += ((2.0f*index)-256.0f);
        }
       // std::cout<<"index = "<<out<<std::endl;
        return true;
    }
    else{
        return false;
    }    
}

void Character::render(GLfloat deltatime)
{
    GLfloat imgX = meta["size"]["w"].asFloat();
    GLfloat imgY = meta["size"]["h"].asFloat();
    GLfloat frameX = frames[frame]["x"].asFloat();
    GLfloat frameY = frames[frame]["y"].asFloat();
    GLfloat size = frames[frame]["w"].asFloat();
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
    }  

    object.ClearUV();
    object.LoadUV(vertices,8);

    tex.UseTexture(GL_TEXTURE0);

    program->UseShader();

    setDirection();
    gforce(deltatime);
    

    model = glm::translate(model,glm::vec3(finalVX,finalVY,0.0f));
    model = glm::scale(model,glm::vec3(scale,1.0f,1.0f));
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniform1i(program->GetDebugLocation(),0);
    glUniform1i(program->GetSamplerLocation(),0);
    
    object.RenderMesh(GL_TRIANGLE_STRIP);
    
    scale=1.0f;
    finalVX=0.0f;
    glUseProgram(0);

}

