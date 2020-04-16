.PHONY: all clean
all : WindowGen.o game.o
	g++ -std=c++17 game.o WindowGen.o -framework OpenGL -framework Cocoa -framework IOKit -lGLEW.2.1.0 -lglfw.3.3 -o game
WindowGen.o: ./source/WindowGen.cpp
	g++ -std=c++17 -I ./include -o WindowGen.o -c ./source/WindowGen.cpp
game.o: ./source/game.cpp
	g++ -std=c++17 -I ./include -o game.o -c ./source/game.cpp
clean:
	rm WindowGen.o
	rm game.o 
	rm game