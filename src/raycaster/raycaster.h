#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "typedefs.h"

// Cast the rays. This includes drawing them in 2D map and on the screen as 3D projection.
void cast_rays();

// Cast the ray that "sees" horizontal intersections with grid.
// Returns 3 values: (pointers to which were passed in arguments)
// - Resulting ray (its end position)
// - The wall this ray stumbled upon
// - Whether or not to ignore this ray COMPLETELY from further calculations
void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *ray_out, unsigned int *wall_out, bool *ignore_out);

// Cast the ray that "sees" vertical intersections with grid.
// Returns 3 values: (pointers to which were passed in arguments)
// - Resulting ray (its end position)
// - The wall this ray stumbled upon
// - Whether or not to ignore this ray COMPLETELY from further calculations
void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *ray_out, unsigned int *wall_out, bool *ignore_out);

#endif  // ifnder RAYCASTER_H