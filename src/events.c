#include <SDL2/SDL.h>
#include "Context.h"
#include "Map.h"
#include "Vector/Vector2.h"
#include "utils.h"
#include "constants.h"
#include "events.h"

void handleEvents()
{
    pollEvents();
    handleKeyboardState();
}

static void handleKeyboardState()
{
    SDL_PumpEvents();
    handlePlayerMovement();
}

static void pollEvents()
{
    while (SDL_PollEvent(&context.event) != 0)
    {
        if (context.event.type == SDL_QUIT)
            context.running = false;
        else if (context.event.type == SDL_MOUSEMOTION)
            handleMouseMotion();
    }
}

static void handleMouseMotion()
{
    context.player.seeAngle += context.event.motion.xrel * MOUSE_SENSITIVITY;
    context.player.seeAngle = NORMALIZE_RADIANS(context.player.seeAngle);
    rendererMoveMouseInWindow(context.renderer, (Vector2i) {
        WINDOW_WIDTH / 2,
        WINDOW_HEIGHT / 2
    });
}

static void handlePlayerMovement()
{
    // walk vectors, magnitude = STEP_LENGTH
    Vector2d forwardVector = {
        cos(context.player.seeAngle) * STEP_LENGTH,
        sin(context.player.seeAngle) * STEP_LENGTH
    };
    Vector2d rightVector = {
        cos(context.player.seeAngle + RADIANS(90)) * STEP_LENGTH,
        sin(context.player.seeAngle + RADIANS(90)) * STEP_LENGTH
    };

    Vector2d playerPositionIfMadeMovement;

    if (
        (context.keystate[SDL_SCANCODE_W] &&
        isPlayerAbleToMoveInDirection(
            Forward, forwardVector,
            rightVector, &playerPositionIfMadeMovement
        )) ||
        (context.keystate[SDL_SCANCODE_S] && 
        isPlayerAbleToMoveInDirection(
            Backward, forwardVector,
            rightVector, &playerPositionIfMadeMovement
        )) ||
        (context.keystate[SDL_SCANCODE_A] && 
        isPlayerAbleToMoveInDirection(
            Left, forwardVector,
            rightVector, &playerPositionIfMadeMovement
        )) ||
        (context.keystate[SDL_SCANCODE_D] && 
        isPlayerAbleToMoveInDirection(
            Right, forwardVector,
            rightVector, &playerPositionIfMadeMovement
        ))
    )
    {
        context.player.position = playerPositionIfMadeMovement;
    }
}

// Checks if player is able to move in a certain direction
// Params: direction, forward and right vectors
// Returns a boolean and a new player position
static bool isPlayerAbleToMoveInDirection(
    Direction direction, Vector2d forward,
    Vector2d right, Vector2d* playerPositionIfMadeMovement
)
{
    assert(playerPositionIfMadeMovement != NULL);
    switch (direction)
    {
        case Forward:
            return getBlockAtPosition(
                context.map,
                Vector2dFloor(*playerPositionIfMadeMovement = Vector2dAdd(
                    context.player.position,
                    forward
                ))
            )->type == BlockTypeAir;
        case Backward:
            return getBlockAtPosition(
                context.map,
                Vector2dFloor(*playerPositionIfMadeMovement = Vector2dSubtract(
                    context.player.position,
                    forward
                ))
            )->type == BlockTypeAir;
        case Left:
            return getBlockAtPosition(
                context.map,
                Vector2dFloor(*playerPositionIfMadeMovement = Vector2dSubtract(
                    context.player.position,
                    right
                ))
            )->type == BlockTypeAir;
        case Right:
            return getBlockAtPosition(
                context.map,
                Vector2dFloor(*playerPositionIfMadeMovement = Vector2dAdd(
                    context.player.position,
                    right
                ))
            )->type == BlockTypeAir;
        default:
            LOGE("isPlayerAbleToMoveInDirection unknown direction!");
            return false;
    }
}

