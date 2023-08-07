#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "constants.h"
#include "typedefs.h"

#define to_radians(x) x * (PI / 180)

bool running = true;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

Player player = {{3, 3}, 0.0f};
const unsigned int map[MAP_WIDTH * MAP_HEIGHT] = {
    1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1
};

void draw_ray_3d(int x, float ray_length);

void init(SDL_Window** window, SDL_Renderer** renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Failed to initialize SDL2 library: %c", SDL_GetError());
	    exit(ERROR_CODE_SDL_INITIALIZATION);
    }
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS, window, renderer) != 0)
    {
        printf("Failed to create window/renderer: %c", SDL_GetError());
        exit(ERROR_CODE_SDL_CREATE_WINDOW_OR_RENDERER);
    }
}

void poll_events()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            running = false;

        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            else if (event.key.keysym.sym == SDLK_w)
            {
                player.pos.x += cos(to_radians(player.angle)) * PLAYER_SPEED;
                player.pos.y += sin(to_radians(player.angle)) * PLAYER_SPEED;
            }
            else if (event.key.keysym.sym == SDLK_a)
            {
                player.pos.x += cos(to_radians(player.angle - 90.0f)) * PLAYER_SPEED;
                player.pos.y += sin(to_radians(player.angle - 90.0f)) * PLAYER_SPEED;
            }
            else if (event.key.keysym.sym == SDLK_s)
            {
                player.pos.x -= cos(to_radians(player.angle)) * PLAYER_SPEED;
                player.pos.y -= sin(to_radians(player.angle)) * PLAYER_SPEED;
            }
            else if (event.key.keysym.sym == SDLK_d)
            {
                player.pos.x += cos(to_radians(player.angle + 90.0f)) * PLAYER_SPEED;
                player.pos.y += sin(to_radians(player.angle + 90.0f)) * PLAYER_SPEED;
            }
            else if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.angle -= PLAYER_ANGLE_DELTA;
                if (player.angle <= 0.0f)
                    player.angle = 360.0f;
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

void draw_map()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int pixel = map[j * MAP_HEIGHT + i];
            // set color depending on what number that is
            if (pixel == 0)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            else if (pixel == 1)
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

            // draw square
            SDL_Rect square = {
                i * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                j * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                MAP_SQUARE_SIZE,
                MAP_SQUARE_SIZE
            };
            SDL_RenderFillRect(renderer, &square);
        }
    }
}

void draw_player()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect player_rect = {
        player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE) - PLAYER_SIZE / 2,
        player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE) - PLAYER_SIZE / 2,
        PLAYER_SIZE,
        PLAYER_SIZE
    };
    SDL_RenderFillRect(renderer, &player_rect);
}

void cast_rays()
{
    float ray_angle = player.angle - (FOV / 2.0f);
    // incremental computation constant
    float d_angle = (float) FOV / GAME_WIDTH;

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        vec2f ray = player.pos;
        float ray_angle_rad = to_radians(ray_angle);
        float sin_ray_angle = sin(ray_angle_rad) / RAYCASTING_PRECISION;
        float cos_ray_angle = cos(ray_angle_rad) / RAYCASTING_PRECISION;

        int wall = 0;
        while (wall == 0)
        {
            ray.x += cos_ray_angle;
            ray.y += sin_ray_angle;

            wall = map[(int) (floor(ray.y) * MAP_WIDTH + floor(ray.x))];
        }

        // calculate ray length
        float ray_length = sqrt(pow(player.pos.x - ray.x, 2) + pow(player.pos.y - ray.y, 2));
        draw_ray_3d(x, ray_length);

        // TODO: fix rendering of rays in 2D
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer,
                           player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           ray.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           ray.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE));

        ray_angle += d_angle;
    }
}

void draw_ray_3d(int x, float ray_length)
{
    int x_screen = x + WINDOW_WIDTH / 2;
    int ray_length_screen = SCALE / ray_length;

    int y1 = GAME_HEIGHT / 2 - ray_length_screen / 2;
    int y2 = y1 + ray_length_screen;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, x_screen, y1, x_screen, y2);
}

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

        printf("%f\n", player.angle);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
