#pragma once
#include "SDL2/SDL.h"
#include "Vector/Vector2.h"
#include "Color/Color.h"

// @brief A wrapper around default SDL Window and Renderer structures
typedef struct
{
    SDL_Window* internal_window;
    SDL_Renderer* internal_renderer;
} Renderer;

Renderer* newRenderer(int width, int height, int flags);
void freeRenderer(Renderer* this);

Vector2i rendererGetWindowDimensions(Renderer* this);

void rendererSetDrawColor(Renderer* this, Color color);
void rendererDrawPixel(Renderer* this, Vector2i point, Color color);
void rendererDrawLine(Renderer* this, Vector2i p1, Vector2i p2, Color color);

void rendererMoveMouseInWindow(Renderer* this, Vector2i pos);

void rendererSaveBuffer(Renderer* this, const char* filename);
void rendererSwapBuffer(Renderer* this);
void rendererClearBuffer(Renderer* this, Color color);

