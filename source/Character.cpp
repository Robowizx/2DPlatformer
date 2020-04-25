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
    ipos= x;
    direction = dir;
    hitFlag = false;
    initialVY = 0.0f;
    finalVY = 0.0f;
    finalVX = 0.0f;
    scale = 1.0f;
    timea = 0.0f;
    timef = 0.0f;
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
    anim_index= 0;
    frame = order[anim_index].asInt();
   // std::cout<<"mirror = "<<frames[frame]["index"].asFloat()<<std::endl;

    LRBT();

    int val=1;
    if(debug){
        val=3;
    }
    for(int i=0;i<val;i++){
        Mesh* x = new Mesh();
        objects.push_back(x);
    }
    objects[0]->CreateMesh(vertices,8,4);
    objects[0]->bindVAO();
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

void Character::ALRBT()
{
    if(direction){
        AL = posx + (2.0f*frames[frame]["L"][1].asFloat());
        AR = posx + 256.0f - (2.0f*frames[frame]["R"][1].asFloat());
        AB = posy + (2.0f*frames[frame]["B"][1].asFloat());
        AT = posy + 256.0f - (2.0f*frames[frame]["T"][1].asFloat());
   }
   else{
        AR = posx - (2.0f*frames[frame]["L"][1].asFloat());
        AL = posx - 256.0f + (2.0f*frames[frame]["R"][1].asFloat());
        AB = posy + (2.0f*frames[frame]["B"][1].asFloat());
        AT = posy + 256.0f - (2.0f*frames[frame]["T"][1].asFloat());
    }
}

void Character::gforce(GLfloat deltatime)
{
    
    LRBT();
    GLfloat diff = B-posy; 
    if(B>bound[2]){
        finalVY = initialVY + (GRAVITY*timea);
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
    
    LRBT();
    if(((keys[GLFW_KEY_LEFT] && direction) || (keys[GLFW_KEY_RIGHT] && (!direction))) && (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]))){
       // std::cout<<"left key = "<<keys[GLFW_KEY_LEFT]<<" right key = "<<keys[GLFW_KEY_RIGHT]<<"direction = "<<direction<<std::endl;
        direction = !direction;
        scale= -1.0f;
        GLfloat index = frames[frame]["index"].asFloat();
        if(!direction){
            //std::cout<<"before posx = "<<posx<<" finalVX = "<<finalVX<<std::endl;
            finalVX = 2.0f*(ipos-index)+256.0f;
            posx += (256.0f-(2.0f*index));
        }
        else{
            //std::cout<<"before posx = "<<posx<<" finalVX = "<<finalVX<<std::endl;
            finalVX = 2.0f*(ipos-index)+256;

            posx += ((2.0f*index)-256.0f);
        }
        //std::cout<<"after posx = "<<posx<<" finalVX = "<<finalVX<<std::endl;
        return true;
    }
    else{
        return false;
    }    
}
void Character::setState(std::string st,GLfloat deltatime){
    state = st;
    order = animation[state]["list"];
    anim_index = 0;
    frame = order[anim_index].asInt();
    if(st != IDLE)
        timef+=deltatime;
    else
        timef=0.0f;    
}

void Character::stateUpdate(GLfloat deltatime)
{   
    if(state == JUMP){
        if(finalVY<=0.0f && B>bound[2]){
            setState(FALL,deltatime);
            setFall(deltatime);
        }
        else{
            setJump(deltatime);
        }
    }
    else if(state == FALL){
       setFall(deltatime);
    }
    else if(state == RUN){
        if(keys[GLFW_KEY_UP]){
            initialVY = 612;
            setState(JUMP,deltatime);
            setJump(deltatime);
        }
        else if(keys[GLFW_KEY_Z]){
            setState(ATTACK_1,deltatime);
            setAttack(deltatime);
        }
        else if(keys[GLFW_KEY_X]){
            setState(ATTACK_2,deltatime);
            setAttack(deltatime);
        }
        else if(((!keys[GLFW_KEY_LEFT]) &&  (!keys[GLFW_KEY_RIGHT])) || (keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]))
        {
            setState(IDLE,deltatime);
        }
        else{
            if(timef>=ANIM_SPEED){
                timef=0.0;
                if(anim_index<(animation[state]["len"].asInt()-1))
                    anim_index++;
                else
                    anim_index=0;         
            }
            else{
             timef+=deltatime;
            }
            frame = order[anim_index].asInt();
            //std::cout<<"anim_index = "<<anim_index<<" frame = "<<frame<<std::endl;
            setRun(deltatime,R_SPEED);
        }    
    }
    else if(state == ATTACK_1 || state == ATTACK_2){
        setAttack(deltatime);
    }
    else{
        //std::cout<<"UP = "<<keys[GLFW_KEY_UP]<<"finalVY = "<<finalVY<<" B = "<<B<<std::endl;
        if(finalVY<0.0f && B>bound[2]){
            setState(FALL,deltatime);
            setFall(deltatime);
        }
        else if(keys[GLFW_KEY_UP]){
            initialVY = 612;
            setState(JUMP,deltatime);
            setJump(deltatime);
        }
        else if((keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT]) && (!setDirection()) && (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]))){
            setState(RUN,deltatime);
            setRun(deltatime,480.0f);
        }
        else if(keys[GLFW_KEY_Z]){
            setState(ATTACK_1,deltatime);
            setAttack(deltatime);
        }
        else if(keys[GLFW_KEY_X]){
            setState(ATTACK_2,deltatime);
            setAttack(deltatime);
        }     
    }
}

void Character::setFall(GLfloat deltatime)
{
    if(timef>=ANIM_SPEED){
        timef = 0.0f;
        if(anim_index<1)
            anim_index++;
        else if(B==bound[2] && (anim_index<(animation[state]["len"].asInt()-1)))
            anim_index++;
        else
            setState(IDLE,deltatime);       
    }
    else
        timef+=deltatime;
    frame = order[anim_index].asInt();
    if(!setDirection())
        setRun(deltatime,J_SPEED); 
}

void Character::setJump(GLfloat deltatime)
{
    if(timef>=ANIM_SPEED){
        timef = 0.0f;
        if(anim_index<(animation[state]["len"].asInt()-1))
            anim_index++;
    }
    else
        timef += deltatime;
    frame = order[anim_index].asInt();
    if(!setDirection())
        setRun(deltatime,J_SPEED);    
}

void Character::setAttack(GLfloat deltatime)
{
            if(timef>=ANIM_SPEED){
                timef=0.0;
                if(anim_index<(animation[state]["len"].asInt()-1))
                    anim_index++;
                else
                {
                   state = IDLE;
                   timef=0.0f;
                   order = animation[state]["list"];
                   anim_index = 0;
                   frame = order[anim_index].asInt();  
                }
                        
            }
            else{
             timef+=deltatime;
            }
            if(frames[frame]["len"].asInt()>1){
                hitFlag=true;
                ALRBT();
            }    
            else
                hitFlag=false;    
            frame = order[anim_index].asInt();
}

void Character::setRun(GLfloat deltatime,GLfloat velX)
{
        
        LRBT();
        if(L>bound[0] && (!direction) && keys[GLFW_KEY_LEFT])
        {
            GLfloat diff = posx-L;
            finalVX = (velX*deltatime);
            posx-=finalVX;
            if((posx-diff)<bound[0]){
                finalVX = (velX*deltatime)-(bound[0]-(posx-diff));
                posx+=(bound[0]-(posx-diff));
            }
        }
        else if(R<bound[1] && direction && keys[GLFW_KEY_RIGHT])
        {
            GLfloat diff = R-posx;
            finalVX = velX*deltatime;
            posx+=finalVX;
            if((posx+diff)>bound[1]){
                finalVX = (velX*deltatime)-((posx+diff)-bound[1]);
                posx-=((posx+diff)-bound[1]);
            }
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

    objects[0]->ClearUV();
    objects[0]->LoadUV(vertices,8);

    tex.UseTexture(GL_TEXTURE0);

    program->UseShader();

    stateUpdate(deltatime);
    //std::cout<<"state = "<<state<<std::endl;
    gforce(deltatime);
    

    model = glm::translate(model,glm::vec3(finalVX,finalVY,0.0f));
    model = glm::scale(model,glm::vec3(scale,1.0f,1.0f));
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniform1i(program->GetDebugLocation(),0);
    
    objects[0]->RenderMesh(GL_TRIANGLE_STRIP);
    
    glUseProgram(0);

    LRBT();
    scale=1.0f;
    finalVX=0.0f;

    if(debug){
        vertices[0]=L;
        vertices[1]=T;
        vertices[2]=R;
        vertices[3]=T;
        vertices[4]=R;
        vertices[5]=B;
        vertices[6]=L;
        vertices[7]=B;

        objects[1]->CreateMesh(vertices,8,4);

        program->UseShader();

        glm::mat4 debugModel(1.0f);
        glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(debugModel));
        glUniform1i(program->GetDebugLocation(),1);

        objects[1]->RenderMesh(GL_LINE_LOOP);

        glUseProgram(0);

        objects[1]->ClearMesh();

        if(frames[frame]["len"].asInt()>1){
            ALRBT();
            vertices[0]=AL;
            vertices[1]=AT;
            vertices[2]=AR;
            vertices[3]=AT;
            vertices[4]=AR;
            vertices[5]=AB;
            vertices[6]=AL;
            vertices[7]=AB;

            objects[2]->CreateMesh(vertices,8,4);

            program->UseShader();

            glm::mat4 debugModel2(1.0f);
            glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(debugModel2));
            glUniform1i(program->GetDebugLocation(),2);

            objects[2]->RenderMesh(GL_LINE_LOOP);

            glUseProgram(0);

            objects[2]->ClearMesh();
        }

    }
}

Character::~Character()
{
    tex.ClearTexture(GL_TEXTURE0);
    for(int i=0;i<objects.size();i++)
        objects[i]->ClearMesh();
        animation.clear();
        frames.clear();
        order.clear();
        meta.clear();
}