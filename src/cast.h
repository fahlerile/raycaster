#pragma once
#include "Map.h"

typedef enum
{
    RayTypeHorizontal = 0,
    RayTypeVertical
} RayType;

void castAndDrawRays();
void castHorizontalAndVerticalRaysAndReturnShorter(double seeAngle, double* length, Vector2d* hitPosition, RayType* type, Block** hitBlock);
void castRay(RayType type, double seeAngle, Vector2d* hitPosition, Block** hitBlock);
void drawRay(size_t col, double length, RayType rayType, Block* hitBlock);

