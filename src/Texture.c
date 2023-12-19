#include <assert.h>
#include "memoryUtils/memoryUtils.h"
#include "stb_image.h"
#include "Texture.h"

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
    assert(this->data != NULL);
    return this;
}

void freeTexture(Texture* this)
{
    xfree(this);
    stbi_image_free(this->data);
}

