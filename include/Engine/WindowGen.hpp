#pragma once

#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

class Window{
    public:
        Window();
        Window(GLint windowWidth,GLint windowHeight);

        int initialise();

        GLint getBufferWidth(){ return bufferWidth; }
        GLint getBufferHeight(){ return bufferHeight; }

        bool getShouldClose(){ return glfwWindowShouldClose(mainWindow); }

        bool* getsKeys() { return keys; }
        bool pause;

        void swapBuffers(){ glfwSwapBuffers(mainWindow); }

        ~Window();

    private:
        GLFWwindow* mainWindow;
        GLint width, height ,bufferWidth, bufferHeight;
        
        bool keys[1024];

        void createCallbacks();
	    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};