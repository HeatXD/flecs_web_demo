#include "all_imports.h"

int main(int argc, char *argv[]) {
  // Setup window
  SDL_Window *window =
      SDL_CreateWindow("Flecs Web Demo!", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_SHOWN);
  // Setup renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  // Done!
  return EXIT_SUCCESS;
}
