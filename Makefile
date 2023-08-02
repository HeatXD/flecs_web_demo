run-web: 
	emcc src/main.c src/flecs/flecs.c -o docs/index.js -std=gnu99 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=cwrap -s MODULARIZE=1 -s EXPORT_NAME="flecs_demo"

run-native: flecs.o
	gcc src/main.c flecs.o -o main -std=gnu99 -lSDL2

flecs.o:
	gcc -c src/flecs/flecs.c -o flecs.o 
