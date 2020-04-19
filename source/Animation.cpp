#include <Engine/Animation.hpp>

Animation::Animation(){}

bool Animation::keyControl(bool* keys, bool lastPressed){
    if (keys[GLFW_KEY_UP]){
        printf("Up\n");
    }

    if (keys[GLFW_KEY_DOWN]){
        printf("Down\n");
    }

    if (keys[GLFW_KEY_LEFT]){
        return true;
    }

    if (keys[GLFW_KEY_RIGHT]){
        return false;
    }
    return lastPressed;
}

Animation::~Animation(){}