#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "stb_image.h"
#include "Renderer.h"
#include "cast.h"
#include "Context.h"
#include "events.h"

Context context;

int main(int argc, char** argv)
{
    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_SetMainReady();
    constructContext();

    while (context.running)
    {
        handleEvents();
        castAndDrawRays();
        rendererSwapBuffer(context.renderer);
    }

    return 0;
}

