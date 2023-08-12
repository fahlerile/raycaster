#ifndef RAYCASTER_UTILS_H
#define RAYCASTER_UTILS_H

// Useful macros

// Convert degree angle to radians
#define to_radians(x) (x * (PI / 180))

// Get decimal part of some number
#define decimal_part(x) ((double) x - floor(x))

// Get map index from ray vector (used in ray casting functions)
#define to_index(vec) (floor(vec.y) * MAP_WIDTH + floor(vec.x))

// Determine if the index is out-of-bounds of map
#define is_oob(x) (x < 0 || x > MAP_WIDTH * MAP_HEIGHT)

#endif