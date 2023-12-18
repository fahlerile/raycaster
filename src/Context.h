#pragma once

#include <SDL2/SDL.h>
#include "Renderer.h"

typedef struct
{
    Renderer* renderer;
    SDL_Event event;
    bool running;
    Player player;
    Map map;
} Context;

void constructContext();
void destructContext();

extern Context context;

