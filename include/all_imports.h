#ifndef _ALL_IMPORTS_H
#define _ALL_IMPORTS_H
// include flecs
#include "flecs.h"
// include SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
// standard libs
#include <stdlib.h>
// include emscripten
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#endif