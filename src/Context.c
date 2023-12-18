#include <stdlib.h>
#include "errorcodes.h"
#include "Context.h"

void constructContext()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL library: %s\n", SDL_GetError());
        exit(SDL_INIT_FAIL);
    }

    context = (Context) {
        .renderer = newRenderer(800, 600, 0),
        .event = {0},
        .running = true
    };

    atexit(destructContext);

    if (context.renderer == NULL)
    {
        fprintf(stderr, "Failed to initialize SDL window or/and renderer: %s\n", SDL_GetError());
        exit(SDL_RENDERER_OR_WINDOW_FAIL);
    }
}

void destructContext()
{
    freeRenderer(context.renderer);
    SDL_Quit();
}

