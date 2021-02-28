all:
	gcc src/main.c -o output/bin/test
web:
	emcc src/main.c -o output/web/test.html
run:
	gcc src/main.c -o output/bin/test
	./output/bin/test 