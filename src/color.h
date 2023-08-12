#ifndef COLOR_H
#define COLOR_H

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color_t;

#define scale_color_by_scalar(color, scalar) ((color_t) {color.r * scalar, color.g * scalar, color.b * scalar, color.a})

#endif  // #ifndef COLOR_H
