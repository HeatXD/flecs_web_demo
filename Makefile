all:
	gcc src/main.c src/flecs.c -Iinclude/ -lSDL2 -o output/bin/test
web:
	emcc src/main.c src/flecs.c -Iinclude/ -lSDL2 -o output/web/index.html
run-bin:
	gcc src/main.c src/flecs.c -Iinclude/ -lSDL2 -o output/bin/test
	./output/bin/test 