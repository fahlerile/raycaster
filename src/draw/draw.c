#include <math.h>
#include <SDL2/SDL.h>

#include "draw.h"
#include "sdl_utils/sdl_utils.h"
#include "ppm_loader/ppm_loader.h"
#include "constants.h"
#include "typedefs.h"
#include "color.h"

extern SDL_Renderer *renderer;
extern const unsigned int map[MAP_WIDTH * MAP_HEIGHT];
extern const ppm_image_t *textures[];
extern player_t player;

// Draw 2D map
void draw_2d_map()
{
    // for every tile in map
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            // determine the tile it is
            int wall = map[j * MAP_HEIGHT + i];

            // if it is 0, set TWO_D_MAP_ZERO_COLOR, else set TWO_D_MAP_BLOCK_COLOR as color
            color_t color = (wall == 0) ? TWO_D_MAP_ZERO_COLOR : TWO_D_MAP_BLOCK_COLOR;
            set_color(color);

            // draw the tile as square
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

// Draw 2D player
void draw_player()
{
    set_color(TWO_D_PLAYER_COLOR);

    // draw player as rectangle
    SDL_Rect player_rect = {
        player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE) - TWO_D_PLAYER_SIZE / 2,
        player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE) - TWO_D_PLAYER_SIZE / 2,
        TWO_D_PLAYER_SIZE,
        TWO_D_PLAYER_SIZE
    };
    SDL_RenderFillRect(renderer, &player_rect);
}

// Draw 3D ray projection
void draw_ray_3d(int x, float ray_length, unsigned int wall, float hit_in_sq_coordinate, bool shade)
{
    // ray projection length, in pixels
    int ray_length_screen = SCALE / ray_length;
    // beginning of ray projection in screen space, y coordinate
    float ray_y_begin = GAME_HEIGHT / 2 - ray_length_screen / 2;

    set_color(THREE_D_CEILING_AND_FLOOR_COLOR);
    SDL_RenderDrawLine(renderer, x, 0, x, ray_y_begin);                                // draw ceiling
    SDL_RenderDrawLine(renderer, x, ray_y_begin + ray_length_screen, x, GAME_HEIGHT);  // draw floor

    // texture to use with this ray
    const ppm_image_t *texture = textures[wall];
    // how much pixels in the screen does one pixel in the texture occupy
    float pixel_height = (float) ray_length_screen / texture->height;
    // determine what column of the texture to draw on the screen
    int texture_column = hit_in_sq_coordinate * texture->width;

    // for each pixel in the texture's height in needed column
    for (int i = 0; i < texture->height; i++)
    {
        color_t texture_pixel = texture->data[ppm_x_y_to_index(texture_column, i, texture->width)];  // get pixel from the texture
        if (shade) texture_pixel = scale_color_by_scalar(texture_pixel, SHADE_CONSTANT);
        float ray_y_end = ray_y_begin + pixel_height;
        set_color(texture_pixel);
        SDL_RenderDrawLine(renderer, x, ray_y_begin, x, ray_y_end);
        ray_y_begin = ray_y_end;
    }
}
