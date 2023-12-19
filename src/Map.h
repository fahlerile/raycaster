#pragma once
#include <stddef.h>
#include "DynamicArray/DynamicArray.h"
#include "Color/Color.h"
#include "Vector/Vector2.h"
#include "Texture.h"

typedef enum
{
    BlockTypeAir = 0,
    BlockTypeColored,
    BlockTypeTextured
} BlockType;

typedef struct
{
    BlockType type;
    union
    {
        Color color;
        Texture* texture;
    };
} Block;

typedef struct
{
    Vector2i dimensions;
    Vector2d defaultPlayerPosition;
    DynamicArray* data;  // Block*
} Map;

Map* newMapFromFile(const char* filename, DynamicArray** textures, DynamicArray** blocks);

