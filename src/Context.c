#include "utils.h"
#include <stdlib.h>
#include "Player.h"
#include "Map.h"
#include "Context.h"
#include "errorcodes.h"
#include "constants.h"

#define MAP_FILENAME "res/maps/1.map"

void constructContext()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL library: %s\n", SDL_GetError());
        exit(SDL_INIT_FAIL);
    }

    context = (Context) {
        .renderer = newRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0),
        .event = {0},
        .running = true
    };

    atexit(destructContext);

    if (context.renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL window or/and renderer: %s\n", SDL_GetError());
        exit(SDL_RENDERER_OR_WINDOW_FAIL);
    }

    context.map = newMapFromFile(
        MAP_FILENAME,
        &context.textures,
        &context.blocks
    );

    context.player = (Player) {
        .position = (Vector2d) {
            context.map->defaultPlayerPosition.x,
            context.map->defaultPlayerPosition.y
        },
        .seeAngle = RADIANS(0),
        .FOV = RADIANS(60)
    };
}

void destructContext()
{
    freeRenderer(context.renderer);
    SDL_Quit();
}

