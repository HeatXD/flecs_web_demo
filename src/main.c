#include <stdio.h>
#include <emscripten.h>

void loop() {
        printf("loop\n");
}

int main() {
        emscripten_set_main_loop(loop, 60, 1);
        printf("End!\n");
        return 0;
}
