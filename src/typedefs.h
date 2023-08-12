#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// A structure for holding two floating-point values.
typedef struct
{
    float x;
    float y;
} vec2f;

// Structure that represents a player. Holds his position and his view angle.
// valid angles are in set [0, 360)
typedef struct
{
    vec2f pos;
    float angle;
} player_t;

#endif  // #ifndef TYPEDEFS_H
