// include flecs
#include "flecs/flecs.h"
// include SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
// standard libs
#include <stdlib.h>
// include emscripten
#include <emscripten.h>

#define MOVESPEED 500

typedef struct Vector2 {
    float x;
    float y;
} Pos2, Size2;

// Create a world 
ecs_world_t* world = NULL;
// SDL Events to make our program interactive
SDL_Event evt;

void MovePlayer(ecs_iter_t* it);
void DrawPlayer(ecs_iter_t* it);
void SetupDraw(ecs_iter_t* it);
void PresentDraw(ecs_iter_t* it);

void main_loop(){
    // Advance Game world
    ecs_progress(world, 0);
    // Check If user want to quit
    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
            case SDL_QUIT: 
                    emscripten_cancel_main_loop();
                break;
        }
    }
}

int run_game(SDL_Renderer* rend) {
  // Init
  world = ecs_init();
  ecs_set_context(world, rend);
  // Set ecs phases to accommodate SDL Draw Functions 
  ecs_entity_t OnUpdate = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t BeforeDraw = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t OnDraw = ecs_new_w_id(world, EcsPhase);
  ecs_entity_t AfterDraw = ecs_new_w_id(world, EcsPhase);
  // Dependencies
  ecs_add_pair(world, BeforeDraw, EcsDependsOn, OnUpdate);
  ecs_add_pair(world, OnDraw, EcsDependsOn, BeforeDraw);
  ecs_add_pair(world, AfterDraw, EcsDependsOn, OnDraw);
  // Register components
  ECS_COMPONENT(world, Pos2);
  ECS_COMPONENT(world, Size2);
  // Define Systems
  ECS_SYSTEM(world, MovePlayer, OnUpdate, Pos2);
  ECS_SYSTEM(world, SetupDraw, BeforeDraw, 0);
  ECS_SYSTEM(world, DrawPlayer, OnDraw, Pos2, Size2);
  ECS_SYSTEM(world, PresentDraw, AfterDraw, 0);
  // Create a new entity
  ECS_ENTITY(world, Player, 0);
  // Set entity start Position and Size
  ecs_set(world, Player, Pos2, {100,100});
  ecs_set(world, Player, Size2, {50,50});
  // Setup gameplay loops
  emscripten_set_main_loop(main_loop, 60, 1);
  return ecs_fini(world);
}

int main(int argc, char *argv[]) {
  // Setup window
  SDL_Window *window = SDL_CreateWindow("Flecs Web Demo!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_SHOWN);
  // Setup renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Run the game
  run_game(renderer);
  // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  // Done!
  return EXIT_SUCCESS;
}

void MovePlayer(ecs_iter_t* it){
    Pos2* p = ecs_field(it, Pos2, 1);
    // Get keyboard state
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < it->count; i++) {   
        if (keystate[SDL_SCANCODE_UP]) {
            p[i].y -= MOVESPEED * it->delta_time;
        }
        if (keystate[SDL_SCANCODE_DOWN]) {
            p[i].y += MOVESPEED * it->delta_time;
        }
        if (keystate[SDL_SCANCODE_LEFT]) {
            p[i].x -= MOVESPEED * it->delta_time;
        }
        if (keystate[SDL_SCANCODE_RIGHT]) {
            p[i].x += MOVESPEED * it->delta_time;
        }        
    }
}

void DrawPlayer(ecs_iter_t* it){
    Pos2* p = ecs_field(it, Pos2, 1);
    Size2* s = ecs_field(it, Size2 , 2);
    SDL_Renderer* rctx = ecs_get_context(it->world);
    // Drawing each player
    for (int i = 0; i < it->count; i++) {
        // Creating a new Rectangle
        SDL_Rect r;
        r.x = p[i].x;
        r.y = p[i].y;
        r.w = s[i].x;
        r.h = s[i].y;
        // Set the rectangle color to red 
        SDL_SetRenderDrawColor(rctx, 255, 0, 0, 255);
        // Render the rectangle
        SDL_RenderFillRect(rctx, &r);
    }
}

void SetupDraw(ecs_iter_t* it){
    SDL_Renderer* rctx = ecs_get_context(it->world);
    // Clearing the current render target with black
    SDL_SetRenderDrawColor(rctx, 0, 0, 0, 1);
    SDL_RenderClear(rctx);
}

void PresentDraw(ecs_iter_t* it){
    SDL_Renderer* rctx = ecs_get_context(it->world);
    // Render the rectangles to the screen
    SDL_RenderPresent(rctx);
}
