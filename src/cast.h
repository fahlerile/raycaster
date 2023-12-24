#pragma once
#include "Map.h"

typedef enum
{
    RayTypeHorizontal = 0,
    RayTypeVertical
} RayType;

/**
 * Cast rays and render them as walls according to context.player data (FOV, position, seeAngle)
 */
void castAndDrawRays();
/**
 * Cast both rays and choose one of them for rendering
 */
static void castHorizontalAndVerticalRaysAndReturnShorter(
    double seeAngle, double* length, Vector2d* hitPosition,
    RayType* type, Block** hitBlock
);
/**
 * Calculate the delta value for raycasting and snap the rayPosition to grid
 */
static void calculateDeltaAndSnapRayToGrid(
    RayType type, double seeAngle,
    bool angleFacingNegX, bool angleFacingNegY,
    double* delta, Vector2d* rayPosition
);
/**
 * Cast a ray, return the position where it bumped into a Block and this Block
 * @param type Ray type
 * @param seeAngle Angle on which to cast ray
 * @param hitPosition A pointer to Vector2d which will store the position where the casted ray bumped into the wall
 * @param hitBlock A pointer to Block* which will store the pointer to a Block into which the ray bumped upon
 * @return int Status code: 0 = OK, -1 = Casted ray is invalid, no changes are made to `hitPosition` and `hitBlock`
 */
static int castRay(
    RayType type, double seeAngle,
    Vector2d* hitPosition, Block** hitBlock
);
/**
 * Render the casted ray as wall
 * @param col Index of a window column on which to draw this ray
 * @param length Length of a casted ray
 * @param rayType Ray type of a casted ray
 * @param hitPosition Position on which the ray "bumped" into a wall
 * @param hitBlock A pointer to the block into which the ray bumped upon
 */
static void drawRay(
    size_t col, double length, RayType rayType, 
    Vector2d hitPosition, Block* hitBlock
);

