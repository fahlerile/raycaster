#include <assert.h>
#include "memoryUtils/memoryUtils.h"
#include "stb_image.h"
#include "Texture.h"
#include "utils.h"

Texture* newTexture(const char* filename, int desiredNChannels)
{
    Texture* this = xmalloc(sizeof(Texture));
    this->data = stbi_load(
        filename,
        &this->dimensions.x,
        &this->dimensions.y, 
        &this->nChannels,
        desiredNChannels
    );
    if (desiredNChannels != 0)
        this->nChannels = desiredNChannels;
    assert(this->data != NULL);
    return this;
}

void freeTexture(Texture* this)
{
    xfree(this);
    stbi_image_free(this->data);
}

Color textureGetPixelAtPosition(Texture* this, Vector2i position)
{
    assert(this->nChannels == 3 || this->nChannels == 4);
    unsigned char *pixel = this->data + this->nChannels * TWO_D_ROW_MAJOR_ARRAY_INDEX_TO_1D_INDEX(position, this->dimensions);
    return (Color) {
        .r = pixel[0],
        .g = pixel[1],
        .b = pixel[2],
        .a = (this->nChannels == 4) ? pixel[3] : 255 
    };
}

