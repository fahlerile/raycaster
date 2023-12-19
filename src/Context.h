#pragma once

#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Player.h"
#include "Map.h"

typedef struct
{
    Renderer* renderer;
    SDL_Event event;
    bool running;
    Player player;
    Map* map;
    DynamicArray* textures;  // Texture*
    DynamicArray* blocks;    // Block
} Context;

void constructContext();
void destructContext();

extern Context context;

