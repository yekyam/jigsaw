jigsaw: ./src/src/*.c ./src/headers/*.h
	gcc -Wall -o jigsaw -I./src/headers ./src/src/*.c