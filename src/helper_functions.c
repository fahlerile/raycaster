#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "constants.h"
#include "typedefs.h"
#include "helper_functions.h"

extern bool running;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;
extern Player player;
extern const unsigned int map[MAP_WIDTH * MAP_HEIGHT];
extern const Color colors[];

#define to_radians(x) (x * (PI / 180))
#define decimal_part(x) ((double) x - floor(x))
#define to_index(vec) (floor(vec.y) * MAP_WIDTH + floor(vec.x))
#define is_oob(x) (x < 0 || x > MAP_WIDTH * MAP_HEIGHT)

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
        if (event.type == SDL_QUIT)
            running = false;

        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            // else if (event.key.keysym.sym == SDLK_w)
            // {
            //     float dx = cos(to_radians(player.angle)) * PLAYER_SPEED;
            //     float dy = sin(to_radians(player.angle)) * PLAYER_SPEED;

            //     // if not trying to go into the block
            //     if (map[(int) (floor(player.pos.y + dy) * MAP_WIDTH + floor(player.pos.x + dx))] == 0)
            //     {
            //         player.pos.x += dx;
            //         player.pos.y += dy;
            //     }
            // }
            // else if (event.key.keysym.sym == SDLK_a)
            // {
            //     float dx = cos(to_radians(player.angle)) * PLAYER_SPEED;
            //     float dy = sin(to_radians(player.angle)) * PLAYER_SPEED;

            //     // if not trying to go into the block
            //     if (map[(int) (floor(player.pos.y - dx) * MAP_WIDTH + floor(player.pos.x + dy))] == 0)
            //     {
            //         player.pos.x += dy;
            //         player.pos.y -= dx;
            //     }
            // }
            // else if (event.key.keysym.sym == SDLK_s)
            // {
            //     float dx = cos(to_radians(player.angle)) * PLAYER_SPEED;
            //     float dy = sin(to_radians(player.angle)) * PLAYER_SPEED;

            //     // if not trying to go into the block
            //     if (map[(int) (floor(player.pos.y - dy) * MAP_WIDTH + floor(player.pos.x - dx))] == 0)
            //     {
            //         player.pos.x -= dx;
            //         player.pos.y -= dy;
            //     }
            // }
            // else if (event.key.keysym.sym == SDLK_d)
            // {
            //     float dx = cos(to_radians(player.angle)) * PLAYER_SPEED;
            //     float dy = sin(to_radians(player.angle)) * PLAYER_SPEED;

            //     // if not trying to go into the block
            //     if (map[(int) (floor(player.pos.y + dx) * MAP_WIDTH + floor(player.pos.x - dy))] == 0)
            //     {
            //         player.pos.x -= dy;
            //         player.pos.y += dx;
            //     }
            // }
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

void draw_map()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int pixel = map[j * MAP_HEIGHT + i];
            Color color = colors[pixel];
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

void cast_rays()
{
    // float ray_angle = player.angle - (FOV / 2.0f);
    float ray_angle = player.angle;
    // incremental computation constant
    float d_angle = (float) FOV / GAME_WIDTH;

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        float theta = to_radians(ray_angle);

        // player in-square coordinates (the decimal part of normal ones)
        float in_sq_x = decimal_part(player.pos.x);
        float in_sq_y = decimal_part(player.pos.y);

        // whether or not to COMPLETELY ignore ray_(h or v) from determining the closest ray
        // e.g to prevent div by 0 at certain angles
        bool ignore_h = false;

        // HORIZONTAL CHECK
        if (ray_angle == 0.0f || ray_angle == 180.0f)  // prevent div by 0
        {
            printf("Skipping horizontal ray (0 or 180)\n");
            ignore_h = true;
            goto skip_horizontal;
        }

        const bool h_ray_facing_pos_y = ray_angle > 0.0f && ray_angle < 180.0f;  // `pos_y` - positive y
        const float xi = (h_ray_facing_pos_y) ? (1 - in_sq_y) / tan(theta) : (-1 + in_sq_y) / tan(theta);
        const float yi = (h_ray_facing_pos_y) ? 1 - in_sq_y : -1 + in_sq_y;
        const float dx = (h_ray_facing_pos_y) ? 1 / tan(theta) : -1 / tan(theta);  // how much to go in `x` direction to go `y_step` in y direction
        int y_step = (h_ray_facing_pos_y) ? 1 : -1;  // change to support other angles
        vec2f h_ray = {player.pos.x + xi, player.pos.y + yi};

        // check if this "snapped" point is in some wall
        // if not, "extend" the ray in while loop
        int index = to_index(h_ray);
        if (is_oob(index) || h_ray.x > MAP_WIDTH || h_ray.y > MAP_HEIGHT) goto skip_horizontal;
        bool h_hit = map[index] != 0;
        while (!h_hit)
        {
            // update ray
            h_ray.x += dx;
            h_ray.y += y_step;

            // check if hit the wall
            // subtracting map width because it skips 1 block if facing negative y
            index = (h_ray_facing_pos_y) ? to_index(h_ray) : to_index(h_ray) - MAP_WIDTH;
            if (is_oob(index)) goto skip_horizontal;  // NOTE: should I undo the ray "extend" because oob?
            h_hit = map[index] != 0;
        }

skip_horizontal:
        float h_ray_length = 0;
        if (!ignore_h)
            h_ray_length = sqrt(pow(player.pos.x - h_ray.x, 2) + pow(player.pos.y - h_ray.y, 2));

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer,
                           player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           h_ray.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           h_ray.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE));

        // // calculate ray length
        // float ray_length = sqrt(pow(player.pos.x - ray.x, 2) + pow(player.pos.y - ray.y, 2));
        // float alpha = ray_angle - player.angle;  // angle between ray and player's view (degrees)
        // float adj_length = ray_length * cos(to_radians(alpha));  // fisheye fix
        // draw_ray_3d(x, adj_length, wall);

        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderDrawLine(renderer,
        //                    player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
        //                    player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
        //                    ray.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
        //                    ray.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE));

        ray_angle += d_angle;
    }
}

void draw_ray_3d(int x, float ray_length, unsigned int wall)
{
    int x_screen = x + WINDOW_WIDTH / 2;
    int ray_length_screen = SCALE / ray_length;

    int y1 = GAME_HEIGHT / 2 - ray_length_screen / 2;
    int y2 = y1 + ray_length_screen;

    SDL_SetRenderDrawColor(renderer, colors[0].r, colors[0].g, colors[0].b, colors[0].a);
    SDL_RenderDrawLine(renderer, x_screen, 0, x_screen, y1);  // draw ceiling
    SDL_RenderDrawLine(renderer, x_screen, y2, x_screen, GAME_HEIGHT);  // draw floor

    Color color = colors[wall];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x_screen, y1, x_screen, y2);  // draw wall
}
