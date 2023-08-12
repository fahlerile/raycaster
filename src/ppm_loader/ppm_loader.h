#ifndef PPM_LOADER_H
#define PPM_LOADER_H

#include "color.h"

// Get index to `ppm_image_t.data` (1D) from 2D indices
#define ppm_x_y_to_index(x, y, width) (y * width + x)

// Structure representing a PPM image.
// `data` is 1-dimensional, use `ppm_x_y_to_index` macro to
// transform 2D indices to 1D index for `data`
typedef struct
{
    int width;
    int height;
    color_t *data;
} ppm_image_t;

// Load PPM image to memory. Allocates memory on the heap, so don't forget
// to call `free_ppm_image` when you are done with the image!
ppm_image_t* load_ppm_image(const char *path);

// Free the PPM image's heap allocated memory.
void free_ppm_image(ppm_image_t *image);

#endif  // ifndef PPM_LOADER_H