#include<Engine/WindowGen.h>

 Window gameWindow;

int main(){
    gameWindow = Window(800,600);
    if(gameWindow.initialise()==1){
        return 1;
    }

    while(!gameWindow.getShouldClose()){

        glfwPollEvents();
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gameWindow.swapBuffers();
    }

    return 0;
}
