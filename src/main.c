#include <stdio.h>
#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "raycaster/raycaster.h"
#include "sdl_utils/sdl_utils.h"
#include "ppm_loader/ppm_loader.h"
#include "draw/draw.h"
#include "constants.h"
#include "typedefs.h"

bool running = true;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

player_t player = {{5.5f, 8.5f}, 0.0f};
const unsigned int map[MAP_WIDTH * MAP_HEIGHT] = {
    #include "maps/map.txt"
};
ppm_image_t *textures[NUM_TEXTURES + 1] = { NULL };  // 0th texture is reserved to avoid ambiguity

int main(int argc, char **argv)
{
    // load textures
    const char *textures_paths[NUM_TEXTURES] = {
        #include "maps/textures.txt"
    };
    for (int i = 1; i < (NUM_TEXTURES + 1); i++)
        textures[i] = load_ppm_image(textures_paths[i - 1]);

    // frametimes are in milliseconds
    int prev_frame_time = 0;
    int this_frame_time = 0;
    int delta_time = 0;

    init(&window, &renderer);

    while (running)
    {
        this_frame_time = SDL_GetTicks();
        delta_time = this_frame_time - prev_frame_time;

        poll_events(delta_time);

        set_color((color_t) {0, 0, 0, 255});
        SDL_RenderClear(renderer);

        draw_map();
        draw_player();
        cast_rays();

        SDL_RenderPresent(renderer);

        prev_frame_time = this_frame_time;
    }

    for (int i = 1; i < (NUM_TEXTURES + 1); i++)
        free_ppm_image(textures[i]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
