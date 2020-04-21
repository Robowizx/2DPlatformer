#include <Engine/Character.hpp>

Character::Character(){}

Character::Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, bool dir, Shader* prg){
    //initialising variables
    posx = x;
    posy = y;
    direction = dir;
    debug = dbug;
    keys = k;
    program = prg; 
    gravity = -5.0f;
    velX = 0.0f;
    initalVY = 0.0f;
    finalVY = 0.0f;
    timea = 0.0f;
    timef = 0.0f;
    state = IDLE;

    //loading texture
    tex = Texture(tfile);
    tex.LoadTexture();
    
    //loading texture metadata
    std::ifstream stream(mfile,std::ifstream::binary);
    if(!stream){
        std::cout<<"could not find file"<<std::endl;
        return;
    }
    Json::Value root;
    stream>>root;
    meta = root["meta"];
    animation = root["animation"];
    frames = root["frames"];
    order = animation[state]["list"];
    frame = order[0].asInt();

    //calculating collision values
    LRBT();

    //stakcking mesh objects and initializing vertices
    meshInit();
    std::cout<<"L = "<<L<<" R = "<<R<<" B = "<<B<<" T = "<<T<<std::endl;
    std::cout<<"list size = "<<Meshlist.size()<<std::endl;
}


void Character::LRBT(){
    
    Json::Value data = frames[frame];
    if(direction){
        L = posx+(2.0f*(GLfloat)data["L"][0].asInt());
        R = posx+256.0f-(2.0f*(GLfloat)data["R"][0].asInt());
        B = posy+(2.0f*(GLfloat)data["B"][0].asInt());
        T = posy+256.0f-(2.0f*(GLfloat)data["T"][0].asInt());
    }
    else{
        L = posx-(2.0f*(GLfloat)data["L"][0].asInt());
        R = posx-256.0f+(2.0f*(GLfloat)data["R"][0].asInt());
        B = posy-(2.0f*(GLfloat)data["B"][0].asInt());
        T = posy-256.0f+(2.0f*(GLfloat)data["T"][0].asInt());
    }
}

void Character::meshInit(){
    for(int i=0;i<8;i++){
        if(i%2!=0)
            vertices[i] +=posx;
        else
            vertices[i]+=posy; 
    }
    int val=1;
    if(debug)
        val=3;
    for(;val>0;val--){
        Mesh x;
        Meshlist.push_back(x);
    }
    Meshlist[0].CreateMesh(vertices,8,4);
    Meshlist[0].bindVAO();    
}

void Character::stateUpdate(){
    
    if(state == RUN){
        // setDirection();
        // if(keys[GLFW_KEY_UP]){
        //     setJump();
        // }
        // else if(keys[GLFW_KEY_Z]){
        //     setAttack1();
        // }
        // else if(keys[GLFW_KEY_X]){
        //     setAttack2();
        // }
        // else if(!keys[GLFW_KEY_LEFT] && !keys[GLFW_KEY_RIGHT]){
        //     setIdle();
        // }
        // else{

        // }

    }
    else if(state == JUMP){

    }
    else if(state == ATTACK_1){

    }
    else if(state == ATTACK_2){

    }
    else{
        if(keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT]){
            //setRun();
        }
         else if(keys[GLFW_KEY_UP]){
            //setJump();
        }
        else if(keys[GLFW_KEY_Z]){
            //setAttack1();
        }
        else if(keys[GLFW_KEY_X]){
            //setAttack2();
        }
    }
}

GLfloat Character::setDirection(){
    
    if(((keys[GLFW_KEY_LEFT] && direction)||(keys[GLFW_KEY_RIGHT] && !direction)) && !(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT])){
        direction = !direction;
        return (GLfloat)(frames[frame]["index"].asInt());
    }    
    
}

void Character::gforce(GLfloat deltatime){

    GLfloat diff = B-posy;
    if(B>bounds[2]){
        finalVY = initalVY + (gravity*timea);
        timea += deltatime;
    }
    else{
        timea=0.0f;
        finalVY = initalVY + (gravity*timea);
    }
    posy += (finalVY * deltatime);
    if((posy+diff)<bounds[2]){
     finalVY = (finalVY*deltatime)-(posy+diff);
     posy -= (posy+diff);
     initalVY=0.0f;    
    }
    else
    {
      initalVY = finalVY;
    }
    
}

void Character::render(GLfloat deltatime){
    
    GLfloat index=0.0f,scale=1.0f,texX,texY,frameX,frameY,size;
    texX = (GLfloat)(meta["size"]["w"].asInt());
    texY = (GLfloat)(meta["size"]["h"].asInt());
    frameX = (GLfloat)(frames[frame]["x"].asInt());
    frameY = (GLfloat)(frames[frame]["Y"].asInt());
    size = (GLfloat)(frames[frame]["w"].asInt());

    //stateUpdate();
    gforce(deltatime);
    index = setDirection();
    if(index){
        if(direction)
            posx-=index;
        else
            posx+=index;
        scale=-1.0f;
    }
    
    Meshlist[0].ClearUV();
    for(int i=0;i<8;i++){
        if(i%2==0){
            if(i<=2)
                vertices[i] = frameX/texX;
            else
                vertices[i] = (frameX+size)/texX;
        }
        else{
            if(i==1 || i==5)
                vertices[i] = 1 - ((frameY+size)/texY);
            else
                vertices[i] = 1 - (frameY/texY);     
        }
    }
    Meshlist[0].LoadUV(vertices,8);

    program->UseShader();
    tex.UseTexture(GL_TEXTURE0);

    glm::mat4 model(1.0f);
    model = glm::translate(model,glm::vec3(index,finalVY,0.0f));
    model = glm::scale(model,glm::vec3(scale,1.0f,1.0f));

    glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
    glUniform1i(program->GetDebugLocation(),0);
    glUniform1i(program->GetSamplerLocation(),0);

    Meshlist[0].RenderMesh(GL_TRIANGLE_STRIP);


    LRBT();
    if(debug){
     vertices[0]=L;
     vertices[1]=B;
     vertices[2]=L;
     vertices[3]=T;
     vertices[4]=R;
     vertices[5]=B;
     vertices[6]=R;
     vertices[7]=T;
     Meshlist[1].CreateMesh(vertices,8,4);

     model = glm::mat4(1.0f);
     glUniformMatrix4fv(program->GetModelLocation(),1,GL_FALSE,glm::value_ptr(model));
     glUniform1i(program->GetDebugLocation(),1);

     Meshlist[1].RenderMesh(GL_LINE_LOOP);

     Meshlist[1].ClearMesh();

    }
    glUseProgram(0);
    
}

Character::~Character(){
    for(int i=0;i< Meshlist.size();i++){
        Meshlist[i].ClearMesh();
    }
    Meshlist.clear();
    tex.ClearTexture(GL_TEXTURE0);
}

