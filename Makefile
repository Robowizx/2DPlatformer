.PHONY: mac clean linux
mac : WindowGen.o Mesh.o Shader.o game.o Texture.o
	g++ -std=c++17 ./build/game.o ./build/WindowGen.o ./build/Mesh.o ./build/Shader.o -I ./include-framework OpenGL -framework Cocoa -framework IOKit -lGLEW.2.1.0 -lglfw.3.3 -o ./build/game
linux: WindowGen.o Mesh.o Shader.o game.o Texture.o
	g++ -std=c++17 ./build/game.o ./build/WindowGen.o ./build/Mesh.o ./build/Shader.o ./build/Texture.o -I ./include -lGL -lGLEW -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -o ./build/game
WindowGen.o: ./source/WindowGen.cpp
	g++ -std=c++17 -I ./include -o ./build/WindowGen.o -c ./source/WindowGen.cpp
Mesh.o: ./source/Mesh.cpp
	g++ -std=c++17 -I ./include -o ./build/Mesh.o -c ./source/Mesh.cpp
Shader.o: ./source/Shader.cpp
	g++ -std=c++17 -I ./include -o ./build/Shader.o -c ./source/Shader.cpp		
game.o: ./source/game.cpp
	g++ -std=c++17 -I ./include -o ./build/game.o -c ./source/game.cpp
Texture.o: ./source/Texture.cpp
	g++ -std=c++17 -I ./include -o ./build/Texture.o -c ./source/Texture.cpp

clean:
	rm ./build/*
