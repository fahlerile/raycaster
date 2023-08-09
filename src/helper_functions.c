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
    float ray_angle = player.angle - (FOV / 2.0f);
    float d_angle = (float) FOV / GAME_WIDTH;  // incremental computation constant, delta angle

    for (int x = 0; x < GAME_WIDTH; x++)
    {
        // limit angles to [0, 360)
        if (ray_angle < 0.0f)
            ray_angle = 360.0f + ray_angle;

        float theta = to_radians(ray_angle);
        float tan_theta = tan(theta);

        // player in-square coordinates (the decimal part of normal ones)
        float in_sq_x = decimal_part(player.pos.x);
        float in_sq_y = decimal_part(player.pos.y);

        // cast horizontal ray
        vec2f h_ray;  // the ray itself
        bool ignore_h;  // whether or not to ignore this ray from calculations
        unsigned int wall_h;  // on what kind of wall this ray stumbled upon
        cast_horizontal_ray(ray_angle, tan_theta, in_sq_y, &h_ray, &wall_h, &ignore_h);

        // cast vertical ray
        vec2f v_ray;
        bool ignore_v;
        unsigned int wall_v;
        cast_vertical_ray(ray_angle, tan_theta, in_sq_x, &v_ray, &wall_v, &ignore_v);

        // choose which ray to use
        vec2f ray;
        float ray_length;
        unsigned int wall;

        if (ignore_h)
        {
            ray = v_ray;
            ray_length = sqrt(pow(player.pos.x - v_ray.x, 2) + pow(player.pos.y - v_ray.y, 2));
            wall = wall_v;
        }
        else if (ignore_v)
        {
            ray = h_ray;
            ray_length = sqrt(pow(player.pos.x - h_ray.x, 2) + pow(player.pos.y - h_ray.y, 2));
            wall = wall_h;
        }
        else  // no ignores
        {
            float h_ray_length = sqrt(pow(player.pos.x - h_ray.x, 2) + pow(player.pos.y - h_ray.y, 2));
            float v_ray_length = sqrt(pow(player.pos.x - v_ray.x, 2) + pow(player.pos.y - v_ray.y, 2));

            if (h_ray_length > v_ray_length)
            {
                ray = v_ray;
                ray_length = v_ray_length;
                wall = wall_v;
            }
            else
            {
                ray = h_ray;
                ray_length = h_ray_length;
                wall = wall_h;
            }
        }

        // fisheye fix
        float alpha = ray_angle - player.angle;  // angle between ray and player's view (degrees)
        float adj_length = ray_length * cos(to_radians(alpha));
        draw_ray_3d(x, adj_length, wall);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer,
                           player.pos.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           player.pos.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           ray.x * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE),
                           ray.y * (MAP_SQUARE_SIZE + MAP_BORDER_SIZE));

        ray_angle += d_angle;
        // limit angles to [0, 360)
        if (ray_angle >= 360.0f)
            ray_angle = ray_angle - 360.0f;
    }
}

void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *h_ray_out, unsigned int *wall_h, bool *ignore_h_out)
{
    // HORIZONTAL CHECK
    // whether or not to COMPLETELY ignore ray_(h or v) from determining the closest ray
    bool ignore_h = false;
    if (ray_angle == 0.0f || ray_angle == 180.0f)  // prevent div by 0
    {
        ignore_h = true;
        goto skip_horizontal;
    }

    const bool h_ray_facing_pos_y = ray_angle > 0.0f && ray_angle < 180.0f;  // `pos_y` - positive y
    const float yi_h = (h_ray_facing_pos_y) ? 1 - in_sq_y : -1 + in_sq_y;
    const float xi_h = yi_h / tan_theta;
    const float dx = (h_ray_facing_pos_y) ? 1 / tan_theta : -1 / tan_theta;  // how much to go in `x` direction to go `y_step` in y direction
    int y_step = (h_ray_facing_pos_y) ? 1 : -1;
    vec2f h_ray = {player.pos.x + xi_h, player.pos.y + yi_h};

    // check if this "snapped" point is in some wall
    // if not, "extend" the ray in while loop
    // TODO: CHECK IF NEED TO SUBTRACT MAP WIDTH HERE IF FACING NEGATIVE Y
    int index = (h_ray_facing_pos_y) ? to_index(h_ray) : to_index(h_ray) - MAP_WIDTH;
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
        if (is_oob(index) || h_ray.x > MAP_WIDTH || h_ray.y > MAP_HEIGHT) goto skip_horizontal;
        h_hit = map[index] != 0;
    }
    unsigned int wall = map[index];

skip_horizontal:
    // return needed values
    *h_ray_out = h_ray;
    *wall_h = wall;
    *ignore_h_out = ignore_h;
}

void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *v_ray_out, unsigned int *wall_v, bool *ignore_v_out)
{
    // VERTICAL CHECK
    bool ignore_v = false;
    if (ray_angle == 90.0f || ray_angle == 270.0f)  // prevent div by 0
    {
        ignore_v = true;
        goto skip_vertical;
    }

    const bool v_ray_facing_pos_x = (ray_angle >= 0.0f && ray_angle < 90.0f) ||
                                    (ray_angle > 270.0f && ray_angle <= 360.0f || ray_angle == 0);
    const float xi_v = (v_ray_facing_pos_x) ? 1 - in_sq_x : -1 + in_sq_x;
    const float yi_v = xi_v * tan_theta;
    const float dy = (v_ray_facing_pos_x) ? tan_theta : -tan_theta;  // how much to go in `y` direction to go `x_step` in x direction
    int x_step = (v_ray_facing_pos_x) ? 1 : -1;
    vec2f v_ray = {player.pos.x + xi_v, player.pos.y + yi_v};

    // check if this "snapped" point is in some wall
    // if not, "extend" the ray in while loop
    int index = (v_ray_facing_pos_x) ? to_index(v_ray) : to_index(v_ray) - 1;
    if (is_oob(index) || v_ray.x > MAP_WIDTH || v_ray.y > MAP_HEIGHT) goto skip_vertical;
    bool v_hit = map[index] != 0;
    while (!v_hit)
    {
        // update ray
        v_ray.x += x_step;
        v_ray.y += dy;

        // check if hit the wall
        // subtracting map width because it skips 1 block if facing negative y
        index = (v_ray_facing_pos_x) ? to_index(v_ray) : to_index(v_ray) - 1;
        if (is_oob(index) || v_ray.x > MAP_WIDTH || v_ray.y > MAP_HEIGHT) goto skip_vertical;
        v_hit = map[index] != 0;
    }
    unsigned int wall = map[index];

skip_vertical:
    // return needed values
    *v_ray_out = v_ray;
    *wall_v = wall;
    *ignore_v_out = ignore_v;
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
