#include <SDL2/SDL.h>
#include "draw.h"
#include "constants.h"
#include "typedefs.h"

extern SDL_Renderer *renderer;
extern const unsigned int map[MAP_WIDTH * MAP_HEIGHT];
extern const color_t colors[];
extern player_t player;

void draw_map()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int pixel = map[j * MAP_HEIGHT + i];
            color_t color = colors[pixel];
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

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

void draw_ray_2d(vec2f ray)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
                       player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                       player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                       ray.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                       ray.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE));
}

void draw_ray_3d(int x, float ray_length, unsigned int wall, bool shade)
{
    int x_screen = x + WINDOW_WIDTH / 2;
    int ray_length_screen = SCALE / ray_length;

    int y1 = GAME_HEIGHT / 2 - ray_length_screen / 2;
    int y2 = y1 + ray_length_screen;

    SDL_SetRenderDrawColor(renderer, colors[0].r, colors[0].g, colors[0].b, colors[0].a);
    SDL_RenderDrawLine(renderer, x_screen, 0, x_screen, y1);  // draw ceiling
    SDL_RenderDrawLine(renderer, x_screen, y2, x_screen, GAME_HEIGHT);  // draw floor

    color_t color = colors[wall];
    if (shade)
    {
        color = (color_t) {color.r * SHADE_CONSTANT,
                           color.g * SHADE_CONSTANT,
                           color.b * SHADE_CONSTANT,
                           color.a * SHADE_CONSTANT};
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x_screen, y1, x_screen, y2);  // draw wall
}
