#include "Renderer.h"
#include "Vector/Vector2.h"
#include "utils.h"
#include <stddef.h>
#include "Context.h"
#include "log.h"
#include "constants.h"
#include "cast.h"

void castAndDrawRays()
{
    double seeAngle = context.player.seeAngle - (context.player.FOV / 2);
    size_t cols = rendererGetWindowDimensions(context.renderer).x;
    double deltaSeeAngle = context.player.FOV / cols;

    double length;
    Vector2d hitPosition;
    RayType type;
    Block* hitBlock;
    for (size_t col = 0; col < cols; col++, seeAngle = NORMALIZE_RADIANS(seeAngle + deltaSeeAngle))
    {
        castHorizontalAndVerticalRaysAndReturnShorter(
            seeAngle, &length, &hitPosition, &type, &hitBlock
        );
        drawRay(col, length, type, hitPosition, hitBlock);
    }
}

static void castHorizontalAndVerticalRaysAndReturnShorter(
    double seeAngle, double* length, Vector2d* hitPosition, RayType* type, Block** hitBlock
)
{
    Vector2d hitPositionH, hitPositionV;
    Block* hitBlockH;
    Block* hitBlockV;

    castRay(RayTypeHorizontal, seeAngle, &hitPositionH, &hitBlockH);
    castRay(RayTypeVertical, seeAngle, &hitPositionV, &hitBlockV);
    double lengthH = EUCLIDEAN_DISTANCE_VECTOR2D(context.player.position, hitPositionH);
    double lengthV = EUCLIDEAN_DISTANCE_VECTOR2D(context.player.position, hitPositionV);

    if (lengthH <= lengthV)
    {
        *length = lengthH * cos(fabs(context.player.seeAngle - seeAngle));  // fisheye fix
        *hitPosition = hitPositionH;
        *type = RayTypeHorizontal;
        *hitBlock = hitBlockH;
    }
    else  // lengthH > lengthV
    {
        *length = lengthV * cos(fabs(context.player.seeAngle - seeAngle));  // fisheye fix
        *hitPosition = hitPositionV;
        *type = RayTypeVertical;
        *hitBlock = hitBlockV;
    }
}

static void castRay(RayType type, double seeAngle, Vector2d* hitPosition, Block** hitBlock)
{
    if (type != RayTypeHorizontal && type != RayTypeVertical)
    {
        LOGE("castRay: Unknown RayType = %i\n", type);
        *hitPosition = (Vector2d) {0, 0};
        *hitBlock = NULL;
        return;
    }

    bool angleFacingNegY = IS_ANGLE_IN_QUADRANT(seeAngle, 3) || IS_ANGLE_IN_QUADRANT(seeAngle, 4);
    bool angleFacingNegX = IS_ANGLE_IN_QUADRANT(seeAngle, 2) || IS_ANGLE_IN_QUADRANT(seeAngle, 3);

    Vector2d rayPosition = context.player.position;
    double delta;
    calculateDeltaAndSnapRayToGrid(
        type, seeAngle, 
        angleFacingNegX, angleFacingNegY, 
        &delta, &rayPosition
    );
    
    Block* block;
    while (
        0 < rayPosition.x && rayPosition.x < context.map->dimensions.x &&  // not OOB
        0 < rayPosition.y && rayPosition.y < context.map->dimensions.y &&
        (block = getBlockAtPosition(  // block stumbled upon == air
            context.map,
            (Vector2i) {
                (angleFacingNegX && type == RayTypeVertical) ? floor(rayPosition.x) - 1 : floor(rayPosition.x), 
                (angleFacingNegY && type == RayTypeHorizontal) ? floor(rayPosition.y) - 1 : floor(rayPosition.y), 
            }
        ))->type == BlockTypeAir
    )
    {
        if (type == RayTypeHorizontal)
        {
            rayPosition.x += delta;
            rayPosition.y += (angleFacingNegY) ? -1 : 1;
        }
        else  // RayTypeVertical
        {
            rayPosition.x += (angleFacingNegX) ? -1 : 1;
            rayPosition.y += delta;
        }
    }

    *hitPosition = rayPosition;
    *hitBlock = block;
    return;
}

static void calculateDeltaAndSnapRayToGrid(
    RayType type, double seeAngle,
    double angleFacingNegX, double angleFacingNegY,
    double* delta, Vector2d* rayPosition
)
{
    double tanSeeAngle = tan(seeAngle);
    double tanPiMinusSeeAngle = tan(M_PI - seeAngle);
    double tan2PiMinusSeeAngle = tan(2*M_PI - seeAngle);

    if (type == RayTypeHorizontal)
    {
        double decimalPartRayPositionY = DECIMAL_PART(rayPosition->y);
        double increment = (angleFacingNegY) ?
            decimalPartRayPositionY / tan2PiMinusSeeAngle :
            (1 - decimalPartRayPositionY) / tanSeeAngle;
        *delta = (angleFacingNegY) ?
            1 / tan2PiMinusSeeAngle :
            1 / tanSeeAngle;

        rayPosition->x += increment;
        rayPosition->y += (angleFacingNegY) ? -decimalPartRayPositionY : (1 - decimalPartRayPositionY);
    }
    else  // RayTypeVertical
    {
        double decimalPartRayPositionX = DECIMAL_PART(rayPosition->x);
        double increment = (angleFacingNegX) ?
            decimalPartRayPositionX * tanPiMinusSeeAngle :
            (1 - decimalPartRayPositionX) * tanSeeAngle;
        *delta = (angleFacingNegX) ?
            tanPiMinusSeeAngle :
            tanSeeAngle;

        rayPosition->x += (angleFacingNegX) ? -decimalPartRayPositionX : (1 - decimalPartRayPositionX);
        rayPosition->y += increment;
    }
}

static void drawRay(size_t col, double length, RayType rayType, Vector2d hitPosition, Block* hitBlock)
{
    assert(hitBlock->type != BlockTypeAir);

    int lengthOfLine = LINE_LENGTH_PROPORTION_CONSTANT / length;
    int startOfLine = (WINDOW_HEIGHT - lengthOfLine) / 2;
    int endOfLine = startOfLine + lengthOfLine;

    rendererDrawLine(
        context.renderer,
        (Vector2i) {col, 0},
        (Vector2i) {col, startOfLine},
        ROOF_COLOR
    );
    rendererDrawLine(
        context.renderer,
        (Vector2i) {col, endOfLine},
        (Vector2i) {col, WINDOW_HEIGHT-1},
        FLOOR_COLOR
    );
    if (hitBlock->type == BlockTypeColored)
    {
        rendererDrawLine(
            context.renderer,
            (Vector2i) {col, startOfLine},
            (Vector2i) {col, endOfLine},
            (rayType == RAY_TYPE_TO_SHADE) ? colorMultiplyByScalar(hitBlock->color, SHADE_CONSTANT) : hitBlock->color
        );
    }
    else  // BlockTypeTextured
    {
        int columnOfTexture = DECIMAL_PART((rayType == RayTypeHorizontal) ? hitPosition.x : hitPosition.y) * hitBlock->texture->dimensions.x;
        double lengthOfTexturePixelProjected = (double) lengthOfLine / hitBlock->texture->dimensions.y;
        for (int i = 0; i < hitBlock->texture->dimensions.y; i++)
        {
            Color color = textureGetPixelAtPosition(hitBlock->texture, (Vector2i) {columnOfTexture, i});
            rendererDrawLine(
                context.renderer,
                (Vector2i) {col, startOfLine + i*lengthOfTexturePixelProjected},
                (Vector2i) {col, startOfLine + (i+1)*lengthOfTexturePixelProjected},
                color
            );
        }
    }
}

