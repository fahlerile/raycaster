#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#define to_radians(x) (x * (PI / 180))
#define decimal_part(x) ((double) x - floor(x))
#define to_index(vec) (floor(vec.y) * MAP_WIDTH + floor(vec.x))
#define is_oob(x) (x < 0 || x > MAP_WIDTH * MAP_HEIGHT)

#include "typedefs.h"

void init(SDL_Window **window, SDL_Renderer **renderer);
void poll_events(int delta_time);

#endif  // ifndef HELPER_FUNCTIONS_H