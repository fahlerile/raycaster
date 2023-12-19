#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "memoryUtils/memoryUtils.h"
#include "fileUtils/fileUtils.h"
#include "stringUtils/stringUtils.h"
#include "stb_image.h"
#include "Map.h"

static void textureFreeCallback(void* p_texture)
{
    freeTexture(*(Texture**) p_texture);
}

Map* newMapFromFile(const char* filename, DynamicArray** textures, DynamicArray** blocks)
{
    FILE* fp = fopen(filename, "r");
    assert(fp != NULL);
    char* line = NULL;
    size_t n = 0;

    Map* this = xmalloc(sizeof(Map));
    this->data = newDynamicArray(25, sizeof(Block*), NULL);

    *textures = newDynamicArray(1, sizeof(Texture*), textureFreeCallback);
    *blocks = newDynamicArray(2, sizeof(Block), NULL);
    
    readLine(&line, &n, fp);
    assert(strcmp(line, "MAP\n") == 0);
    
    size_t width, height;
    double defaultX, defaultY;
    fscanf(fp, "%zu %zu %lf %lf\n", &width, &height, &defaultX, &defaultY);

    readLine(&line, &n, fp);
    assert(strcmp(line, "#textures\n") == 0);

    // if this loop never hits "#blocks" it will eventually fail on `newTexture`
    while (readLine(&line, &n, fp) == 0 && strcmp(line, "#blocks\n") != 0)
    {
        line[strlen(line)-1] = '\0';  // cut \n
        Texture* texture = newTexture(line, 3);
        addToDynamicArray(*textures, &texture);
    }

    while (true)
    {
        unsigned char type;
        if (fscanf(fp, "%hhu", &type) == 0)
            break;

        Block block;

        if (type == 0)
            block.type = BlockTypeAir;
        else if (type == 1)
        {
            block.type = BlockTypeColored;
            unsigned char r, g, b;
            fscanf(fp, "%hhu %hhu %hhu\n", &r, &g, &b);
            block.color = (Color) {r, g, b, 255};
        }
        else if (type == 2)
        {
            block.type = BlockTypeTextured;
            size_t index;
            fscanf(fp, "%zu\n", &index);
            block.texture = *(Texture**) indexDynamicArray(*textures, index);
        }

        addToDynamicArray(*blocks, &block);
    }

    readLine(&line, &n, fp);
    assert(strcmp(line, "#map\n") == 0);

    while (readLine(&line, &n, fp) == 0)
    {
        line[strlen(line)-1] = '\0';  // cut \n
        DynamicArray* blockIndices = splitString(line, ",");
        for (size_t i = 0; i < blockIndices->size; i++)
        {
            char* token = *(char**) indexDynamicArray(blockIndices, i);
            addToDynamicArray(
                this->data, 
                &(Block**){indexDynamicArray(*blocks, atoi(token))}
            );
        }
    }

    xfree(line);

    this->dimensions = (Vector2i) {width, height};
    this->defaultPlayerPosition = (Vector2d) {
        defaultX, defaultY
    };

    return this;
}

