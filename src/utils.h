#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include "Vector/Vector2.h"
#include "log.h"

#define RADIANS(x) ((M_PI / 180) * (x))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define DECIMAL_PART(x) ((x) - floor(x))

static inline double EUCLIDEAN_DISTANCE_VECTOR2D(Vector2d a, Vector2d b)
{
    Vector2d v = Vector2dSubtract(a, b);
    return sqrt(v.x * v.x + v.y * v.y);
}

static inline size_t TWO_D_ROW_MAJOR_ARRAY_INDEX_TO_1D_INDEX(Vector2i position, Vector2i dimensions)
{
    assert(0 <= position.x && position.x < dimensions.x);
    assert(0 <= position.y && position.y < dimensions.y);
    return (position.y * dimensions.x) + position.x;
}

// Assumes that angle is positive and lies in [0; 2pi]
static inline bool IS_ANGLE_IN_QUADRANT(double rad, int quadrantIndex)
{
    switch (quadrantIndex)
    {
        case 1:
            return ((rad >= 0 && rad < RADIANS(90)) || rad == RADIANS(360));
            break;
        case 2:
            return (rad >= RADIANS(90) && rad < RADIANS(180));
            break;
        case 3:
            return (rad >= RADIANS(180) && rad < RADIANS(270));
            break;
        case 4:
            return (rad >= RADIANS(270) && rad < RADIANS(360));
            break;
        default:
            LOGE("IS_ANGLE_IN_QUADRANT wrong quadrant index!\n");
            return false;
    }
}

