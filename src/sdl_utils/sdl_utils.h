#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "typedefs.h"

void init(SDL_Window **window, SDL_Renderer **renderer);
void poll_events(int delta_time);

void set_color(color_t color);

#endif  // ifndef HELPER_FUNCTIONS_H