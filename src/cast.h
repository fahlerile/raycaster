#pragma once
#include "Map.h"

typedef enum
{
    RayTypeHorizontal = 0,
    RayTypeVertical
} RayType;

void castAndDrawRays();
static void castHorizontalAndVerticalRaysAndReturnShorter(
    double seeAngle, double* length, Vector2d* hitPosition,
    RayType* type, Block** hitBlock
);
static void calculateDeltaAndSnapRayToGrid(
    RayType type, double seeAngle,
    double angleFacingNegX, double angleFacingNegY,
    double* delta, Vector2d* rayPosition
);
static void castRay(
    RayType type, double seeAngle,
    Vector2d* hitPosition, Block** hitBlock
);
static void drawRay(
    size_t col, double length, RayType rayType, 
    Vector2d hitPosition, Block* hitBlock
);

