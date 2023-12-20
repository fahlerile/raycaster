#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION

#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "stb_image.h"
#include "Renderer.h"
#include "cast.h"
#include "Context.h"

Context context;

void pollEvents();
void updateContext();
void castAndDrawRays();

int main(int argc, char** argv)
{
    SDL_SetMainReady();
    constructContext();

    castAndDrawRays();
    while (context.running)
    {
        pollEvents();
        updateContext();
        rendererSwapBuffer(context.renderer);
    }

    return 0;
}

void pollEvents()
{
    while (SDL_PollEvent(&context.event) != 0)
    {
        if (context.event.type == SDL_QUIT)
            context.running = false;
    }
}

void updateContext()
{

}

