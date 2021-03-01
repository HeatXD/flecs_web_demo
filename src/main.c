#include "../include/all_imports.h" 
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>

#define MOVESPEED 500

typedef struct Vector2 {
    float x;
    float y;
} Position, Size2;

int run_game(SDL_Renderer* renderer);
void MovePlayer(ecs_iter_t* it);
void DrawPlayer(ecs_iter_t* it);
void SetupDraw(ecs_iter_t* it);
void PresentDraw(ecs_iter_t* it);
void main_loop(void);

// flag for the gameloop
bool running = false;
// Create a world 
ecs_world_t* world = NULL;
// SDL Events to make our program interactive
SDL_Event evt;

int main(void) {
    // Setup window
    SDL_Window* window = SDL_CreateWindow(
        "Flecs Web Demo!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        900, 900,
        SDL_WINDOW_SHOWN);
    // Setup renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Setup and run the game
    run_game(renderer);
    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    // Done!
    return EXIT_SUCCESS;
}

int run_game(SDL_Renderer* renderer){
    // Create a flecs world
    world = ecs_init();
    // Set Sdl renderer as ecs context
    ecs_set_context(world, renderer);
    // Register components
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Size2);
    // Specify pipeline phases
    ECS_TAG(world, OnUpdate);
    ECS_TAG(world, BeforeDraw);
    ECS_TAG(world, OnDraw);
    ECS_TAG(world, AfterDraw);
    // Create the pipeline
    ECS_PIPELINE(world, ExecOrder, OnUpdate, BeforeDraw, OnDraw, AfterDraw)
    // Set the world to the new pipeline
    ecs_set_pipeline(world, ExecOrder);
    // Define Systems
    ECS_SYSTEM(world, MovePlayer, OnUpdate, Position);
    ECS_SYSTEM(world, SetupDraw, BeforeDraw, 0);
    ECS_SYSTEM(world, DrawPlayer, OnDraw, Position, Size2);
    ECS_SYSTEM(world, PresentDraw, AfterDraw, 0);
    // Create a new entity
    ECS_ENTITY(world, Player, 0);
    // Set entity start Position and Size
    ecs_set(world, Player, Position, {100,100});
    ecs_set(world, Player, Size2, {50,50});
    // Setup gameplay loops
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #endif
    #ifndef __EMSCRIPTEN__
        running = true;
        while (running == true) {main_loop();}
    #endif
    // Cleanup flecs
    return ecs_fini(world);
}

void main_loop(void){
    // Advance Game world
    ecs_progress(world, 0);
    // Check If user want to quit
    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
            case SDL_QUIT: 
                #ifdef __EMSCRIPTEN__
                    emscripten_cancel_main_loop();
                #endif
                #ifndef __EMSCRIPTEN__
                    running = false;
                #endif
                break;
        }
    }
}

void MovePlayer(ecs_iter_t* it){
    Position* p = ecs_column(it, Position, 1);
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
    Position* p = ecs_column(it, Position, 1);
    Size2* s = ecs_column(it, Size2 , 2);
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