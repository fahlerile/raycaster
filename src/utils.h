#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include "Vector/Vector2.h"

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
    int index = (position.y * dimensions.x) + position.x;
    assert(index < (dimensions.x * dimensions.y));
    return index;
}

