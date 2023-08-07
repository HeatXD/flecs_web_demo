run-web: 
	emcc src/main.c src/flecs/flecs.c -o docs/index.html 

run-native: flecs.o
	gcc src/main.c flecs.o -o main -std=gnu99 -lSDL2

flecs.o:
	gcc -c src/flecs/flecs.c -o flecs.o 
