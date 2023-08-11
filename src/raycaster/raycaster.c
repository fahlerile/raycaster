#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "sdl_utils/sdl_utils.h"
#include "raycaster.h"
#include "draw/draw.h"
#include "constants.h"
#include "typedefs.h"

extern SDL_Renderer *renderer;
extern player_t player;
extern const unsigned int map[MAP_WIDTH * MAP_HEIGHT];

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
        bool shade = false;

        if (ignore_h)
        {
            ray = v_ray;
            ray_length = sqrt(pow(player.pos.x - v_ray.x, 2) + pow(player.pos.y - v_ray.y, 2));
            wall = wall_v;
            shade = true;
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
                shade = true;
            }
            else
            {
                ray = h_ray;
                ray_length = h_ray_length;
                wall = wall_h;
            }
        }

        // fisheye fix
        float alpha = player.angle - ray_angle;
        float adj_length = ray_length * cos(to_radians(alpha));
        draw_ray_3d(x, adj_length, wall, shade);
        draw_ray_2d(ray);

        ray_angle += d_angle;
        // limit angles to [0, 360)
        if (ray_angle >= 360.0f)
            ray_angle = ray_angle - 360.0f;
    }
}

void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *h_ray_out, unsigned int *wall_h_out, bool *ignore_h_out)
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
    const float yi_h = (h_ray_facing_pos_y) ? 1 - in_sq_y : -in_sq_y;
    const float xi_h = yi_h / tan_theta;
    const float dx = (h_ray_facing_pos_y) ? 1 / tan_theta : -1 / tan_theta;  // how much to go in `x` direction to go `y_step` in y direction
    int y_step = (h_ray_facing_pos_y) ? 1 : -1;
    vec2f h_ray = {player.pos.x + xi_h, player.pos.y + yi_h};

    // check if this "snapped" point is in some wall
    // if not, "extend" the ray in while loop
    // TODO: CHECK IF NEED TO SUBTRACT MAP WIDTH HERE IF FACING NEGATIVE Y
    int index = (h_ray_facing_pos_y) ? to_index(h_ray) : to_index(h_ray) - MAP_WIDTH;
    if (is_oob(index) || h_ray.x > MAP_WIDTH || h_ray.y > MAP_HEIGHT || h_ray.x < 0 || h_ray.y < 0)
    {
        ignore_h = true;
        goto skip_horizontal;
    }
    bool h_hit = map[index] != 0;
    while (!h_hit)
    {
        // update ray
        h_ray.x += dx;
        h_ray.y += y_step;

        // check if hit the wall
        // subtracting map width because it skips 1 block if facing negative y
        index = (h_ray_facing_pos_y) ? to_index(h_ray) : to_index(h_ray) - MAP_WIDTH;
        if (is_oob(index) || h_ray.x > MAP_WIDTH || h_ray.y > MAP_HEIGHT || h_ray.x < 0 || h_ray.y < 0)
        {
            ignore_h = true;
            goto skip_horizontal;
        }
        h_hit = map[index] != 0;
    }
    unsigned int wall = map[index];

skip_horizontal:
    // return needed values
    *h_ray_out = h_ray;
    *wall_h_out = wall;
    *ignore_h_out = ignore_h;
}

void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *v_ray_out, unsigned int *wall_v_out, bool *ignore_v_out)
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
    const float xi_v = (v_ray_facing_pos_x) ? 1 - in_sq_x : -in_sq_x;
    const float yi_v = xi_v * tan_theta;
    const float dy = (v_ray_facing_pos_x) ? tan_theta : -tan_theta;  // how much to go in `y` direction to go `x_step` in x direction
    int x_step = (v_ray_facing_pos_x) ? 1 : -1;
    vec2f v_ray = {player.pos.x + xi_v, player.pos.y + yi_v};

    // check if this "snapped" point is in some wall
    // if not, "extend" the ray in while loop
    int index = (v_ray_facing_pos_x) ? to_index(v_ray) : to_index(v_ray) - 1;
    if (is_oob(index) || v_ray.x > MAP_WIDTH || v_ray.y > MAP_HEIGHT || v_ray.x < 0 || v_ray.y < 0) goto skip_vertical;
    bool v_hit = map[index] != 0;
    while (!v_hit)
    {
        // update ray
        v_ray.x += x_step;
        v_ray.y += dy;

        // check if hit the wall
        // subtracting map width because it skips 1 block if facing negative y
        index = (v_ray_facing_pos_x) ? to_index(v_ray) : to_index(v_ray) - 1;
        if (is_oob(index) || v_ray.x > MAP_WIDTH || v_ray.y > MAP_HEIGHT || v_ray.x < 0 || v_ray.y < 0) goto skip_vertical;
        v_hit = map[index] != 0;
    }
    unsigned int wall = map[index];

skip_vertical:
    // return needed values
    *v_ray_out = v_ray;
    *wall_v_out = wall;
    *ignore_v_out = ignore_v;
}
