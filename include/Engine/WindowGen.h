#pragma once

#include<iostream>
#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

class Window{
    public:
        Window();
        Window(GLint windowWidth,GLint windowHeight);

        int initialise();

        GLint getBufferWidth(){ return bufferWidth; }
        GLint getBufferHeight(){ return bufferHeight; }

        bool getShouldClose(){ return glfwWindowShouldClose(mainWindow); }

        void swapBuffers(){ glfwSwapBuffers(mainWindow); }

        ~Window();

    private:
        GLFWwindow* mainWindow;
        GLint width, height ,bufferWidth, bufferHeight;
};