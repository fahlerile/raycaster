#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

void init(SDL_Window** window, SDL_Renderer** renderer);
void poll_events();
void draw_map();
void draw_player();
void cast_rays();
void draw_ray_3d(int x, float ray_length, unsigned int wall);

#endif  // ifndef HELPER_FUNCTIONS_H