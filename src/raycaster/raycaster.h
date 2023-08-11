#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "typedefs.h"

void cast_rays();
void cast_horizontal_ray(float ray_angle, float tan_theta, float in_sq_y,
                         vec2f *h_ray_out, unsigned int *wall_h_out, bool *ignore_h_out);
void cast_vertical_ray(float ray_angle, float tan_theta, float in_sq_x,
                       vec2f *v_ray_out, unsigned int *wall_v_out, bool *ignore_v_out);

#endif  // ifnder RAYCASTER_H