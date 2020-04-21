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
    change=0.0f;
    scale = 1.0f;
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
    L = posx+(2.0f*(GLfloat)data["L"][0].asInt());
    R = posx+256.0f-(2.0f*(GLfloat)data["R"][0].asInt());
    B = posy+(2.0f*(GLfloat)data["B"][0].asInt());
    T = posy+256.0f-(2.0f*(GLfloat)data["T"][0].asInt());
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
        setDirection();
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

void Character::setDirection(){
    
    if(((keys[GLFW_KEY_LEFT] && direction)||(keys[GLFW_KEY_RIGHT] && !direction)) && !(keys[GLFW_KEY_LEFT] && keys[GLFW_KEY_RIGHT])){
        direction = !direction;
        scale = -1.0f;
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
    if(posy-diff<bounds[2]){
     finalVY    
    }
}

void Character::render(GLfloat deltatime){
    
    stateUpdate();
    gforce(deltatime);
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
    }
}

