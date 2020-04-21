#include <Engine/Character.hpp>

Character::Character(){}

Character::Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, int dir){
    //initialising variables
    posx = x;
    posy = y;
    direction = dir;
    debug = dbug;
    keys = k;
    modeloc=0;
    gravity = -5.0f;
    velX = 0.0f;
    initalVY = 0.0f;
    finalVY = 0.0f;
    timea = 0.0f;
    change=0.0f;
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
}