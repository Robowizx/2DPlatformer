#include <Engine/Character.hpp>

Character::Character()
{}

Character::Character(GLfloat x,GLfloat y,char* mfile,char* tfile,char* dmfile, char* dtfile, bool wh, bool dbug, bool* k, bool dir, Shader* prg)
{
    posx = x;
    posy = y;
    iposx =x;
    iposy = y;
    program = prg;
    debug = dbug;
    keys = k;   
    ipos= x;
    direction = dir;
    who =wh;
    Dmeta = dmfile;
    hitflag = false;
    initialVY = 0.0f;
    finalVY = 0.0f;
    finalVX = 0.0f;
    scale = 1.0f;
    timea = 0.0f;
    timef = 0.0f;
    hurt_speed = 0.0f;
    state = IDLE;
    model = glm::mat4(1.0f);
    
    if(direction)
        idir = 1.0f;
    else
        idir = -1.0f;    

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

    Dtex = Texture(dtfile);
    Dtex.LoadTexture();

    std::ifstream stream(mfile,std::ios::binary);
    Json::Value root;
    stream >> root;
    animation = root["animation"];
    frames = root["frames"];
    meta = root["meta"];
    order = animation[state]["list"];
    anim_index= 0;
    frame = order[anim_index].asInt();

    LRBT();

    int val=1;
    if(debug){
        val=2;
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
    GLfloat size;
    if(state == DEATH)
        size = 512.0f;
    else
        size = 256.0f;    
    if(direction){
        L = posx + (2.0f*frames[frame]["L"][0].asFloat());
        R = posx + size - (2.0f*frames[frame]["R"][0].asFloat());
        B = posy + (2.0f*frames[frame]["B"][0].asFloat());
        T = posy + size - (2.0f*frames[frame]["T"][0].asFloat());
   }
   else{
        R = posx - (2.0f*frames[frame]["L"][0].asFloat());
        L = posx - size + (2.0f*frames[frame]["R"][0].asFloat());
        B = posy + (2.0f*frames[frame]["B"][0].asFloat());
        T = posy + size - (2.0f*frames[frame]["T"][0].asFloat());
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
        GLfloat diff = B-posy;   
        if(B>bound[2]){
            finalVY = initialVY + (GRAVITY*timea);
            timea += deltatime;
        }
        else{
            timea = 0.0f;

            if(initialVY>0.0f)
                finalVY = initialVY;
            else
                finalVY = 0.0f;    
        }
        initialVY = finalVY;

        posy += (finalVY*deltatime);
    
        if((posy+diff)<bound[2]){

            finalVY = (finalVY*deltatime)-((posy+diff)-bound[2]);
            posy -= ((posy+diff)-bound[2]);
        }
        else{
            finalVY = (finalVY*deltatime);
        }
        if(state == DEATH)
            finalVY /= 2.0f;
}

bool Character::setDirection(bool manual){
    
    LRBT();
    if((keys == nullptr && manual)||(keys!=nullptr && ((keys[GLFW_KEY_LEFT] && direction) || (keys[GLFW_KEY_RIGHT] && (!direction))) && (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT])))){

        direction = !direction;
        scale= -1.0f;
        GLfloat index = frames[frame]["index"].asFloat();
        if(!direction)
            posx += (256.0f-(2.0f*index));
        else
            posx += ((2.0f*index)-256.0f);
        finalVX = (2.0f*ipos)+(idir*(256-(2.0f*index)));
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
        timef = deltatime;
    else
        timef=0.0f;    
}

void Character::stateUpdate(GLfloat deltatime)
{   
    
    if(state == WIN){
        return;
    }
    else if(state == DEATH){
        doDeath(deltatime);
    }
    else if(state == HURT){
        hitflag = false;
        setHurt(deltatime);
    }
    else if(state == FALL){
       setFall(deltatime);
    }
    else if(state == JUMP){
        if(finalVY<=0.0f && B>bound[2]){
            setState(FALL,deltatime);
            setFall(deltatime);
        }
        else{
            setJump(deltatime);
        }
    }
    else if(state == RUN){
        
        if(keys != nullptr){
            if(keys[GLFW_KEY_UP]){
                initialVY = J_SPEED;
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
            else
                goto manual;
        }
        else{
          manual:  
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
            if(!setDirection(false))
                setRun(deltatime,R_SPEED);
        }    
    }
    else if(state == ATTACK_1 || state == ATTACK_2){
        setAttack(deltatime);
    }
    else{
        if(finalVY<0.0f && B>bound[2]){
            setState(FALL,deltatime);
            setFall(deltatime);
        }
        if(keys != nullptr)
        {
            if(keys[GLFW_KEY_UP]){
                initialVY = J_SPEED;
                setState(JUMP,deltatime);
                setJump(deltatime);
            }
            else if((keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT]) && (!setDirection(false)) && (!(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT]))){
                setState(RUN,deltatime);
                setRun(deltatime,R_SPEED);
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
    LRBT();
}

void Character::setHurt(GLfloat deltatime)
{
    if(timef>=0.09f){
        timef=0.0f;
        if(anim_index < (animation[state]["len"].asInt()-1))
            anim_index++;
        else{
            hurt_speed = 0.0f;
            setState(IDLE,deltatime);
        }        
    }
    else
        timef+=deltatime;
    frame = order[anim_index].asInt();
    LRBT();
    if(L>bound[0] && ((direction && hurt_speed<0.0f) || ((!direction) && hurt_speed>0.0f)))
    {
        GLfloat diff = L-posx;
            finalVX = (hurt_speed*deltatime);
            posx-=abs(finalVX);
            if((posx+diff)<bound[0]){
                if(hurt_speed>0.0f)
                    finalVX = (hurt_speed*deltatime)-(bound[0]-(posx+diff));
                else
                    finalVX = (hurt_speed*deltatime)+(bound[0]-(posx+diff));    
                posx+=(bound[0]-(posx+diff));  
            }
    }
    else if(R<bound[1] && ((direction && hurt_speed>0.0f) || ((!direction) && hurt_speed<0.0f)))
    {
            GLfloat diff = R-posx;
            finalVX = hurt_speed*deltatime;
            
            posx+=abs(finalVX);
            if((posx+diff)>bound[1]){
                if(hurt_speed>0.0f)
                    finalVX = (hurt_speed*deltatime)-((posx+diff)-bound[1]);
                else
                    finalVX = (hurt_speed*deltatime)+((posx+diff)-bound[1]);    
                posx-=((posx+diff)-bound[1]);
            }
    }
    finalVX *= idir;
    
}

void Character::setFall(GLfloat deltatime)
{
    if(timef>=ANIM_SPEED){
        timef = 0.0f;
        if(anim_index<1)
            anim_index++;
        else if(B==bound[2] && (anim_index<(animation[state]["len"].asInt()-1))){
                anim_index++;
        }    
        else if(B==bound[2]){
            if(keys!=nullptr){
                if(keys[GLFW_KEY_Z])
                    setState(ATTACK_1,deltatime);
                else if(keys[GLFW_KEY_X])
                    setState(ATTACK_2,deltatime);
                else
                    setState(IDLE,deltatime);    
            }
            else        
                setState(IDLE,deltatime);
        }           
    }
    else
        timef+=deltatime;
    frame = order[anim_index].asInt();
    if(keys != nullptr){
        if(!setDirection(false))
            setRun(deltatime,RJ_SPEED);
    } 
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
    if(!setDirection(false))
        setRun(deltatime,RJ_SPEED);    
}

void Character::setAttack(GLfloat deltatime)
{
            if(timef>=ANIM_SPEED){
                timef=0.0;
                if(anim_index<(animation[state]["len"].asInt()-1))
                    anim_index++;
                else{
                    if(keys!=nullptr){
                        if((state==ATTACK_1 && keys[GLFW_KEY_Z])||(state==ATTACK_2 && keys[GLFW_KEY_X]))
                            anim_index=0;
                        else{
                           hitflag = false;
                         setState(IDLE,deltatime); 
                        }    
                    }
                    else{
                        hitflag = false;
                        setState(IDLE,deltatime); 
                    }     
                }    
            }
            else
             timef+=deltatime;
            
            if(frames[frame]["len"].asInt()>1){
                hitflag=true;
                ALRBT();
            }    
            else
                hitflag=false;       
            frame = order[anim_index].asInt();
}

void Character::setRun(GLfloat deltatime,GLfloat velX)
{
        LRBT();
        if(L>bound[0] && (!direction) && (keys== nullptr ||keys[GLFW_KEY_LEFT]))
        {
            GLfloat diff = L-posx;
            finalVX = (velX*deltatime);
            posx-=finalVX;
            if((posx+diff)<bound[0]){
                finalVX = (velX*deltatime)-(bound[0]-(posx+diff));
                posx+=(bound[0]-(posx+diff));  
            }
        }
        else if(R<bound[1] && direction && (keys==nullptr || keys[GLFW_KEY_RIGHT]))
        {
            GLfloat diff = R-posx;
            finalVX = velX*deltatime;
            posx+=finalVX;
            if((posx+diff)>bound[1]){
                finalVX = (velX*deltatime)-((posx+diff)-bound[1]);
                posx-=((posx+diff)-bound[1]);
            }
        }
        finalVX *= idir;
}

void Character::setDeath(GLfloat deltatime)
{
    state = DEATH;

    std::ifstream stream(Dmeta,std::ios::binary);
    Json::Value root;
    stream >> root;
    animation = root["animation"];
    frames = root["frames"];
    meta = root["meta"];

    if(who)
        order = animation["hero"]["list"];
    else
        order = animation["enemy"]["list"];
    anim_index = 0;
    frame = order[anim_index].asInt();
    timef = deltatime;
    
    GLfloat DXindex = 2.0f*frames[frame]["DXindex"].asFloat();
    GLfloat DYindex = 2.0f*frames[frame]["DYindex"].asFloat();
    if(direction)
        posx+=DXindex;
    else
        posx=posx-DXindex;
    posy+=DYindex;    
    model = glm::translate(model,glm::vec3((idir*DXindex)-iposx,(DYindex-iposy),0.0f));
    model = glm::scale(model,glm::vec3(2.0f,2.0f,1.0f));
           
}

void Character::doDeath(GLfloat deltatime)
{
    if(timef > 0.14f)
    {
        timef = 0.0f;
        if(anim_index < 9)
            anim_index++;
        else
            state = LOSE;    
    }
    else
    {
        timef += deltatime;
    }
    frame = order[anim_index].asInt();
    
}

void Character::render(GLfloat deltatime)
{
    if(state == LOSE)
        return;
    stateUpdate(deltatime);

    GLfloat imgX = meta["size"]["w"].asFloat();
    GLfloat imgY = meta["size"]["h"].asFloat();
    GLfloat frameX = frames[frame]["x"].asFloat();
    GLfloat frameY = frames[frame]["y"].asFloat();
    GLfloat size = frames[frame]["w"].asFloat();
    
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

    if(state == DEATH || state == LOSE)
        Dtex.UseTexture(GL_TEXTURE0);
    else    
        tex.UseTexture(GL_TEXTURE0);

    program->UseShader();

    gforce(deltatime);
    LRBT();
    
    model = glm::translate(model,glm::vec3(finalVX,finalVY,0.0f));
    model = glm::scale(model,glm::vec3(scale,1.0f,1.0f));
    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniform1i(program->GetDebugLocation(),0);
    
    objects[0]->RenderMesh(GL_TRIANGLE_STRIP);
    
    glUseProgram(0);

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

            objects[1]->CreateMesh(vertices,8,4);

            program->UseShader();

            glm::mat4 debugModel2(1.0f);
            glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(debugModel2));
            glUniform1i(program->GetDebugLocation(),2);

            objects[1]->RenderMesh(GL_LINE_LOOP);

            glUseProgram(0);

            objects[1]->ClearMesh();
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