game: renderer.o
	#g++ renderer.o main.o -I~/dev/SDL/include -L~/dev/SDL/lib -lSDL2main -o game
	g++ renderer.o -lSDL2 -o game -std=c++20

renderer.o: renderer.cpp
	g++ -c renderer.cpp

#main.o: main.cpp
#	g++ -c main.cpp
