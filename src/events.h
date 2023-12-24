#pragma once
#include "Vector/Vector2.h"

typedef enum
{
    Forward = 0,
    Backward,
    Left,
    Right
} Direction;

void handleEvents();
static void handleKeyboardState();
static void pollEvents();
static void handlePlayerMovement();
static void handleCameraMovement();
/**
 * Check if player is able to move in specified direction
 * @param direction Direction
 * @param forward A forward vector (looks in the same way as camera does)
 * @param right A right vector
 * @param playerPositionIfMadeMovement A pointer to Vector2d that will store the location of the player if he made the movement
 */
static bool isPlayerAbleToMoveInDirection(
    Direction direction, Vector2d forward,
    Vector2d right, Vector2d* playerPositionIfMadeMovement
);

