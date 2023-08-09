#include <stdio.h>
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

player_t player = {{7.5f, 7.25f}, 225.0f};
const unsigned int map[MAP_WIDTH * MAP_HEIGHT] = {
    #include "map.txt"
};
const color_t colors[] = {
    #include "colors.txt"
};

int main(int argc, char **argv)
{
    // milliseconds
    int prev_frame_time = 0;
    int this_frame_time = 0;
    int delta_time = 0;

    init(&window, &renderer);

    while (running)
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        poll_events(delta_time);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_map();
        draw_player();
        cast_rays();

        SDL_RenderPresent(renderer);

        prev_frame_time = this_frame_time;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
