#ifndef PPM_LOADER_H
#define PPM_LOADER_H

#include "color.h"

#define ppm_x_y_to_index(x, y, width) (y * width + x)

typedef struct
{
    int width;
    int height;
    color_t *data;
} ppm_image_t;

ppm_image_t* load_ppm_image(const char *path);
void free_ppm_image(ppm_image_t *image);

#endif  // ifndef PPM_LOADER_H