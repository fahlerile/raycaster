#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "sdl_utils.h"
#include "raycaster/raycaster.h"
#include "constants.h"
#include "typedefs.h"
#include "utils.h"

extern bool running;
extern SDL_Event event;
extern SDL_Renderer *renderer;
extern player_t player;
extern const unsigned int map[MAP_WIDTH * MAP_HEIGHT];

void init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Failed to initialize SDL2 library: %c", SDL_GetError());
	    exit(ERROR_CODE_SDL_INITIALIZATION);
    }
    if ((*window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS)) == NULL)
    {
        printf("Failed to create window: %c", SDL_GetError());
        exit(ERROR_CODE_SDL_CREATE_WINDOW);
    }
    if ((*renderer = SDL_CreateRenderer(*window, -1, RENDERER_FLAGS)) == NULL)
    {
        printf("Failed to create renderer: %c", SDL_GetError());
        exit(ERROR_CODE_SDL_CREATE_RENDERER);
    }
}

void poll_events(int delta_time)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;

        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            else if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s ||
                     event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d)
            {
                float speed = PLAYER_SPEED * delta_time;
                float player_angle_rad = to_radians(player.angle);
                float dx = cos(player_angle_rad) * speed;
                float dy = sin(player_angle_rad) * speed;
                vec2f after_movement;

                if (event.key.keysym.sym == SDLK_w)
                    after_movement = (vec2f) {player.pos.x + dx, player.pos.y + dy};
                else if (event.key.keysym.sym == SDLK_s)
                    after_movement = (vec2f) {player.pos.x - dx, player.pos.y - dy};

                else if (event.key.keysym.sym == SDLK_a)
                    after_movement = (vec2f) {player.pos.x + dy, player.pos.y - dx};
                else if (event.key.keysym.sym == SDLK_d)
                    after_movement = (vec2f) {player.pos.x - dy, player.pos.y + dx};

                bool is_going_to_be_inside_wall = map[(int) (to_index(after_movement))] != 0;

                if (!is_going_to_be_inside_wall)
                    player.pos = after_movement;
            }

            else if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.angle -= PLAYER_ANGLE_DELTA;
                if (player.angle <= -1.0f)
                    player.angle = 359.0f;
            }
            else if (event.key.keysym.sym == SDLK_RIGHT)
            {
                player.angle += PLAYER_ANGLE_DELTA;
                if (player.angle >= 360.0f)
                    player.angle = 0.0f;
            }
        }
    }
}

void set_color(color_t color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
