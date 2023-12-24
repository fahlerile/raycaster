#pragma once
#include "Vector/Vector2.h"

typedef struct
{
    Vector2d position;
    double seeAngle;  // angle from OX to player's view ray
    double FOV;
} Player;

