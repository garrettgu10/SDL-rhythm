game:
	g++ *.cpp -o play -l SDL2-2.0.0 -l SDL2_mixer -l SDL2_ttf -std=c++0x -O3

run: game
	./play