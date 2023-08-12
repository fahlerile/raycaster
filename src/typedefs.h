#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef struct
{
    float x;
    float y;
} vec2f;

// NOTE: use angles in [0, 360)
typedef struct
{
    vec2f pos;
    float angle;
} player_t;

#endif  // #ifndef TYPEDEFS_H
