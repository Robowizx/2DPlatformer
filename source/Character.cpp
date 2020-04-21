#include <Engine/Character.hpp>

Character::Character(){}

Character::Character(GLfloat x, GLfloat y, char* mfile,char* tfile, bool dbug, bool* k, int dir,GLint mloc){
    posx = x;
    posy = y;
    direction = dir;
    debug = dbug;
    keys = k;
    modeloc = mloc;
    gravity = -5.0f;
    velX = 0.0f;
    initalVY = 0.0f;
    finalVY = 0.0f;
    timea = 0.0f;
    change=0.0f;
    state = IDLE;
    frame = 0;
    tex = Texture(mfile);
    tex.LoadTexture();
}