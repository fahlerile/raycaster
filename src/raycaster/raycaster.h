#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "typedefs.h"

void cast_rays();
void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *ray_out, unsigned int *wall_out, bool *ignore_out);
void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *ray_out, unsigned int *wall_out, bool *ignore_out);

#endif  // ifnder RAYCASTER_H