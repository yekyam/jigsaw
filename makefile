jigsaw: ./src/src/*.cpp ./src/headers/*.hpp
	g++ -Wall -o jigsaw -I./src/headers ./src/src/*.cpp -std=c++20