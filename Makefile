all:
	gcc src/main.c src/flecs.c -Iinclude/ -o output/bin/test
web:
	emcc src/main.c src/flecs.c -Iinclude/ -o output/web/index.html
run:
	gcc src/main.c src/flecs.c -Iinclude/ -o output/bin/test
	./output/bin/test 