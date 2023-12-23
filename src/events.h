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
static void handleMouseMotion();
static void handlePlayerMovement();
static bool isPlayerAbleToMoveInDirection(
    Direction direction, Vector2d forward,
    Vector2d right, Vector2d* playerPositionIfMadeMovement
);

