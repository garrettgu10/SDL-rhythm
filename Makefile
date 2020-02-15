game:
	g++ *.cpp -o play -l SDL2-2.0.0 -l SDL2_mixer -std=c++0x

run: game
	./play