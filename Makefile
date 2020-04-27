.PHONY: mac clean linux
mac: build game.o
	g++ -std=c++17 ./build/game.o ./build/WindowGen.o ./build/Mesh.o ./build/Texture.o ./build/Shader.o ./build/Character.o -L ./lib/MacOS -framework OpenGL -framework Cocoa -framework IOKit -lGLEW -lglfw3 -ljsoncpp -o ./build/game
linux: build game.o
	g++ -std=c++17 ./build/game.o ./build/WindowGen.o ./build/Mesh.o ./build/Shader.o ./build/Texture.o ./build/Character.o -L ./lib/Linux -lGL -lGLEW -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -ljsoncpp -o ./build/game
WindowGen.o: ./source/WindowGen.cpp
	g++ -std=c++17 -I ./include -o ./build/WindowGen.o -c ./source/WindowGen.cpp
Mesh.o: ./source/Mesh.cpp
	g++ -std=c++17 -I ./include -o ./build/Mesh.o -c ./source/Mesh.cpp
Shader.o: ./source/Shader.cpp
	g++ -std=c++17 -I ./include -o ./build/Shader.o -c ./source/Shader.cpp		
game.o: WindowGen.o Character.o ./source/game.cpp
	g++ -std=c++17 -I ./include -o ./build/game.o -c ./source/game.cpp
Texture.o: ./source/Texture.cpp
	g++ -std=c++17 -I ./include -o ./build/Texture.o -c ./source/Texture.cpp
Character.o: Mesh.o Texture.o Shader.o ./source/Character.cpp
	g++ -std=c++17 -I ./include -o ./build/Character.o -c ./source/Character.cpp
build: 
	mkdir build	
clean:
	rm ./build/*
