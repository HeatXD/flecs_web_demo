run: flecs.o
	gcc src/main.c flecs.o -o main.exe -std=gnu99 -lWs2_32 -Ldep -lSDL2-static

flecs.o:
	gcc -c src/flecs/flecs.c -o flecs.o 