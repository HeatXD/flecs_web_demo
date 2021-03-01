#include "../include/all_imports.h" 

typedef struct Vector2 {
    float x;
    float y;
} Position, Size2;

typedef struct RenderCtx{
    SDL_Renderer* render;
}RenderCtx;

int run_game(SDL_Renderer* renderer);
void MovePlayer(ecs_iter_t* it);
void DrawPlayer(ecs_iter_t* it);
void SetupDraw(ecs_iter_t* it);
void PresentDraw(ecs_iter_t* it);

int main(void) {
    // Setup window
    SDL_Window* window = SDL_CreateWindow(
        "Flecs Web Demo!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        500, 500,
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
    ecs_world_t* world = ecs_init();
    // Register components
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Size2);
    ECS_COMPONENT(world, RenderCtx);
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
    ECS_SYSTEM(world, SetupDraw, BeforeDraw, SYSTEM:RenderCtx);
    ECS_SYSTEM(world, DrawPlayer, OnDraw, Position, Size2, SYSTEM:RenderCtx);
    ECS_SYSTEM(world, PresentDraw, AfterDraw, SYSTEM:RenderCtx);
    // Set Render Context
    ecs_set(world, SetupDraw, RenderCtx, {.render = renderer});
    ecs_set(world, DrawPlayer, RenderCtx, {.render = renderer});
    ecs_set(world, PresentDraw, RenderCtx, {.render = renderer});
    // Create a new entity
    ECS_ENTITY(world, Player, 0);
    // Set entity start Position and Size
    ecs_set(world, Player, Position, {30,30});
    ecs_set(world, Player, Size2, {50,50});

    return ecs_fini(world);
}
void MovePlayer(ecs_iter_t* it){

}

void DrawPlayer(ecs_iter_t* it){
    Position* p = ecs_column(it, Position, 1);
    Size2* s = ecs_column(it, Size2 , 2);
    RenderCtx* rctx = ecs_column(it, RenderCtx, 3);
    // Drawing each player
    for (int i = 0; i < it->count; i++) {
        // Creating a new Rectangle
        SDL_Rect r;
        r.x = p[i].x;
        r.y = p[i].y;
        r.w = s[i].x;
        r.h = s[i].y;
        // Set the rectangle color to red 
        SDL_SetRenderDrawColor(rctx->render, 255, 0, 0, 255);
        // Render the rectangle
        SDL_RenderFillRect(rctx->render, &r);
    }
}
void SetupDraw(ecs_iter_t* it){
    RenderCtx* rctx = ecs_column(it, RenderCtx, 1);

    for (int i = 0; i < it->count; i++) {
        // Clearing the current render target with black
        SDL_SetRenderDrawColor(rctx->render, 0, 0, 0, 1);
        SDL_RenderClear(rctx->render);
    }
}
void PresentDraw(ecs_iter_t* it){
    RenderCtx* rctx = ecs_column(it, RenderCtx, 1);

    for (int i = 0; i < it->count; i++){
        // Render the rectangles to the screen
        SDL_RenderPresent(rctx->render);
    }
}
