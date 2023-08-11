#ifndef RAYCASTER_UTILS_H
#define RAYCASTER_UTILS_H

#define to_radians(x) (x * (PI / 180))
#define decimal_part(x) ((double) x - floor(x))
#define to_index(vec) (floor(vec.y) * MAP_WIDTH + floor(vec.x))
#define is_oob(x) (x < 0 || x > MAP_WIDTH * MAP_HEIGHT)

#endif