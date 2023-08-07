#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef struct
{
    float x;
    float y;
} vec2f;

typedef struct
{
    vec2f pos;
    float angle;
} Player;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

#endif  // #ifndef TYPEDEFS_H