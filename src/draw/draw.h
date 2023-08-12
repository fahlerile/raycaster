#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include <stdbool.h>
#include "typedefs.h"

// Draw 2D map
void draw_map();

// Draw 2D player
void draw_player();

// Draw 3D ray projection
void draw_ray_3d(int x, float ray_length, unsigned int wall, float hit_in_sq_coordinate, bool shade);

// Draw 2D ray
void draw_ray_2d(vec2f ray);

#endif  // ifndef DRAW_FUCTIONS_H