#pragma once
#include <stddef.h>
#include "Vector/Vector2.h"

typedef struct
{
    Vector2i dimensions;
    int nChannels;
    unsigned char* data;
} Texture;

Texture* newTexture(const char* filename, int desiredNChannels);
void freeTexture(Texture* this);

