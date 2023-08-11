#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ppm_loader.h"
#include "typedefs.h"
#include "constants.h"

ppm_image_t* load_ppm_image(const char *path)
{
    FILE* fp = fopen(path, "rb");
    assert(fp != NULL);

    // `getline` resizes the buffer in case it is smaller than actual line
    char *line_buf = malloc(sizeof(char) * MAX_PPM_SIZE_BYTES);

    // get line with ppm version/type
    fgets(line_buf, MAX_PPM_SIZE_BYTES, fp);
    assert(strcmp(line_buf, "P6\n") == 0);

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
    assert(strcmp(line_buf, "255\n") == 0);

    // allocate memory for the image data
    color_t *data = malloc(sizeof(color_t) * height * width);

    // get line with image data
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

    free(line_buf);
    fclose(fp);

    ppm_image_t *image = malloc(sizeof(ppm_image_t));
    *image = (ppm_image_t) {
        width,
        height,
        data
    };

    return image;
}

void free_ppm_image(ppm_image_t *image)
{
    free(image->data);
    free(image);
}
