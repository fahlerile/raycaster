#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "color.h"

// Initializes SDL window and renderer objects
void init(SDL_Window **window, SDL_Renderer **renderer);

// Poll OS events (keypress, exit, etc.)
void poll_events(int delta_time);

// Set the color for drawing (wrapper function that uses my `color_t` type)
void set_color(color_t color);

#endif  // ifndef HELPER_FUNCTIONS_H