#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <stdbool.h>
#include "typedefs.h"

void draw_map();
void draw_player();

void draw_ray_3d(int x, float ray_length, unsigned int wall, float hit_in_sq_coordinate, bool shade);
void draw_ray_2d(vec2f ray);

#endif  // ifndef DRAW_FUCTIONS_H