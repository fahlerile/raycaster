#include <SDL2/SDL.h>
#include "memoryUtils/memoryUtils.h"
#include "Renderer.h"

/**
 * @brief Construct a new `Renderer`
 *
 * @param width width of a window
 * @param height height of a window
 * @param flags SDL flags to pass to `SDL_CreateWindowAndRenderer`
 * @return Renderer* a pointer to constructed `Renderer`
 */
Renderer* newRenderer(int width, int height, int flags)
{
    SDL_Window* int_window;
    SDL_Renderer* int_renderer;
    if (SDL_CreateWindowAndRenderer(width, height, flags, &int_window, &int_renderer) != 0)
        return NULL;

    Renderer* renderer = xmalloc(sizeof(Renderer));
    *renderer = (Renderer) {
        .internal_window = int_window,
        .internal_renderer = int_renderer,
    };
    return renderer;
}

void freeRenderer(Renderer* this)
{
    xfree(this);
}

Vector2i rendererGetWindowDimensions(Renderer* this)
{
    int x, y;
    SDL_GetWindowSize(this->internal_window, &x, &y);
    return (Vector2i) {x, y};
}

void rendererSetDrawColor(Renderer* this, Color color)
{
    SDL_SetRenderDrawColor(this->internal_renderer,
                           color.r, color.g, color.b, color.a);
}

void rendererDrawPixel(Renderer* this, Vector2i point, Color color)
{
    rendererSetDrawColor(this, color);
    SDL_RenderDrawPoint(this->internal_renderer, point.x, point.y);
}

// @brief Save current buffer state as BMP
void rendererSaveBuffer(Renderer* this, const char* filename)
{
    Vector2i dimensions = rendererGetWindowDimensions(this);
    SDL_Surface* shot = SDL_CreateRGBSurface(0, dimensions.x, dimensions.y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(this->internal_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, shot->pixels, shot->pitch);
    SDL_SaveBMP(shot, filename);
    SDL_FreeSurface(shot);
}

void rendererSwapBuffer(Renderer* this)
{
    SDL_RenderPresent(this->internal_renderer);
}

void rendererClearBuffer(Renderer* this, Color color)
{
    rendererSetDrawColor(this, color);
    SDL_RenderClear(this->internal_renderer);
}

