#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "typedefs.h"

void init(SDL_Window** window, SDL_Renderer** renderer);
void poll_events(int delta_time);

void draw_map();
void draw_player();

void cast_rays();
void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *h_ray_out, unsigned int *wall_h_out, bool *ignore_h_out);
void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *v_ray_out, unsigned int *wall_v_out, bool *ignore_v_out);
void draw_ray_3d(int x, float ray_length, unsigned int wall, bool shade);

#endif  // ifndef HELPER_FUNCTIONS_H