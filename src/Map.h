#pragma once
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
    double defaultPlayerSeeAngle;
    DynamicArray* data;  // Block*
} Map;

/**
 * @param filename Path to the .map file
 * @param textures A pointer to uninitialized DynamicArray that will store textures
 * @param textures A pointer to uninitialized DynamicArray that will store block information
 */
Map* newMapFromFile(const char* filename, DynamicArray** textures, DynamicArray** blocks);
Block* getBlockAtPosition(Map* this, Vector2i position);

