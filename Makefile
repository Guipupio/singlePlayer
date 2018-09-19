FLAGS=-lncurses -lpthread -lportaudio -std=c++11

all: model

model: jogo_main.cpp jogoSP.cpp jogoSP.hpp 01-playback.cpp 01-playback.hpp
	g++ -ofuncionapf jogo_main.cpp jogoSP.cpp 01-playback.cpp  $(FLAGS)
