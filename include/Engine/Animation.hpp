#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Animation{
    public:
        Animation();

        bool keyControl(bool* keys, int key);

        ~Animation();

    private:
};