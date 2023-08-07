#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "helper_functions.h"
#include "constants.h"
#include "typedefs.h"

bool running = true;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

Player player = {{3, 3}, 270.0f};
const unsigned int map[MAP_WIDTH * MAP_HEIGHT] = {
    #include "map.txt"
};

int main(int argc, char **argv)
{
    init(&window, &renderer);

    while (running)
    {
        poll_events();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_map();
        draw_player();
        cast_rays();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
