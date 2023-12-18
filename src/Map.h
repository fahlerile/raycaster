#pragma once
#include <stddef.h>

typedef struct
{
    size_t width, height;
    Block* data;
} Map;

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
    }
} Block;

