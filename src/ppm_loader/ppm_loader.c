#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "ppm_loader.h"
#include "color.h"
#include "constants.h"

// Load PPM image to memory. Allocates memory on the heap, so don't forget
// to call `free_ppm_image` when you are done with the image!
ppm_image_t* load_ppm_image(const char *path)
{
    // open the file
    FILE* fp = fopen(path, "rb");
    assert(fp != NULL);

    // create buffer for the lines of the file
    char line_buf[MAX_PPM_SIZE_BYTES];

    // get line with ppm version/type
    fgets(line_buf, MAX_PPM_SIZE_BYTES, fp);
    // assert that the format used is byte-written PPM (P6)
    assert(strcmp(line_buf, "P6\n") == 0 || strcmp(line_buf, "P6\r\n") == 0);

    while (fgets(line_buf, MAX_PPM_SIZE_BYTES, fp) && line_buf[0] == '#');  // skip all comments

    // split line with (width, height)
    char *token = strtok(line_buf, " ");
    unsigned int dimensions[2];  // width, height
    int i = 0;
    while (token != NULL)
    {
        dimensions[i] = atoi(token);
        i++;
        token = strtok(NULL, " ");
    }

    int width = dimensions[0];
    int height = dimensions[1];

    // get line with max color value
    fgets(line_buf, MAX_PPM_SIZE_BYTES, fp);
    // assert that the max color value is 255 (unsigned char upper limit)
    assert(strcmp(line_buf, "255\n") == 0 || strcmp(line_buf, "255\r\n") == 0);

    // allocate memory for the image data
    color_t *data = malloc(sizeof(color_t) * height * width);

    // get line with image data, fill the `data` array with it
    // see https://ru.wikipedia.org/wiki/Portable_anymap for PPM format reference
    fgets(line_buf, MAX_PPM_SIZE_BYTES, fp);
    unsigned int pixel_index = 0;
    for (int i = 0, n = strlen(line_buf) - 2; i < n; i += 3)  // i - byte index
    {
        color_t color = {
            (unsigned char) line_buf[i],
            (unsigned char) line_buf[i+1],
            (unsigned char) line_buf[i+2],
            255
        };
        data[pixel_index] = color;
        pixel_index++;
    }

    // close file
    fclose(fp);

    // allocate memory for the image structure and fill it with values
    ppm_image_t *image = malloc(sizeof(ppm_image_t));
    *image = (ppm_image_t) {
        width,
        height,
        data
    };

    // return the pointer to this image structure
    return image;
}

// Free the PPM image's heap allocated memory.
void free_ppm_image(ppm_image_t *image)
{
    free(image->data);
    free(image);
}
