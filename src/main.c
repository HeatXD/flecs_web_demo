#include "all_imports.h"

void one_iter() {
  // Can render to the screen here, etc.
  printf("one iteration");
}

int main() {
  emscripten_set_main_loop(one_iter, 0, 1);
}
