#include <Engine/Animation.hpp>
#include <Engine/WindowGen.hpp>

Animation::Animation(){}

bool Animation::keyControl(bool* keys,int key){
    return keys[key];
}

Animation::~Animation(){}