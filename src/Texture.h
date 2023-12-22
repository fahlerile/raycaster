#pragma once
#include "Vector/Vector2.h"
#include "Color/Color.h"

typedef struct
{
    Vector2i dimensions;
    int nChannels;
    unsigned char* data;
} Texture;

Texture* newTexture(const char* filename, int desiredNChannels);
void freeTexture(Texture* this);

Color textureGetPixelAtPosition(Texture* this, Vector2i position);

